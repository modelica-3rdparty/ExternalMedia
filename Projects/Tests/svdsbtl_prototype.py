"""
Prototype of three ways to use the CoolProp 8 SVDSBTL tables in ExternalMedia.

The script compares, for water, CO2 and R134a, against plain HEOS:

  A  SVDSBTL plus finite differences of the tables (the first
     implementation, commit 5966387). cp, cv, kappa, beta, ddph, ddhp and
     the saturation derivatives come from 4 extra table look-ups.
  B  The table only inverts the inputs: it returns T, rho and the phase.
     HEOS is then evaluated directly at (T, rho) with the phase imposed,
     so CoolProp skips its own iteration and phase check. All properties
     and analytical derivatives come from this HEOS state.
  C  Like B, plus one Newton step on HEOS so that the state matches the
     input pair (p, h) exactly (to the precision of one Newton step).

Two-phase states and saturation properties:

  B  The table gives T_sat(p) and the saturated densities. HEOS is
     evaluated on the liquid and vapour sides at (T_sat, rho_l) and
     (T_sat, rho_v). The quality follows from h, the saturation
     derivatives from the Clausius-Clapeyron equation.
  C  Like B, plus one Newton step on the phase equilibrium
     (p_l = p, p_v = p, g_l = g_v) in the unknowns T, rho_l, rho_v.

Run with CoolProp 8.0.0:

    pip install CoolProp==8.0.0
    python svdsbtl_prototype.py            # accuracy tables (markdown)
    python svdsbtl_prototype.py --timing   # also Python timings

The first run builds the SVDSBTL tables (minutes per fluid), later runs
load them from ~/.CoolProp/SVDTables.
"""

import math
import sys
import time

import CoolProp.CoolProp as CP

FLUIDS = ["Water", "CO2", "R134a"]

# Relative step for the finite differences of variant A (as in 5966387)
REL_STEP = 1e-5
H_SCALE_J_KG = 1e5

NAN = float("nan")


# ---------------------------------------------------------------------------
# Small helpers
# ---------------------------------------------------------------------------

def rel_err(value, ref):
    """Relative error |value - ref| / |ref|, NaN if either is missing."""
    if value is None or ref is None:
        return NAN
    if not (math.isfinite(value) and math.isfinite(ref)):
        return NAN
    if ref == 0.0:
        return abs(value)
    return abs(value - ref) / abs(ref)


def is_twophase(phase):
    return phase == CP.iphase_twophase


# ---------------------------------------------------------------------------
# Reference: plain HEOS
# ---------------------------------------------------------------------------

def ref_ph(H, p_Pa, h_J_kg, transport=True):
    """Plain HEOS p-h flash, the same outputs as postStateChange."""
    H.unspecify_phase()
    H.update(CP.HmassP_INPUTS, h_J_kg, p_Pa)
    out = {"T": H.T(), "rho": H.rhomass(), "p": H.p(), "h": H.hmass(),
           "phase": H.phase()}
    if is_twophase(H.phase()):
        x = H.Q()
        out["x"] = x
        out["ddph"] = H.first_two_phase_deriv(CP.iDmass, CP.iP, CP.iHmass)
        out["ddhp"] = H.first_two_phase_deriv(CP.iDmass, CP.iHmass, CP.iP)
        # extend_twophase: linear interpolation of the saturated values
        for key, par in (("cp", CP.iCpmass), ("cv", CP.iCvmass), ("a", CP.ispeed_sound)):
            l = H.saturated_liquid_keyed_output(par)
            v = H.saturated_vapor_keyed_output(par)
            out[key] = l + x * (v - l)
    else:
        out.update(single_phase_outputs(H, transport))
    return out


def single_phase_outputs(H, transport=True):
    """All outputs ExternalMedia needs from a single-phase HEOS state."""
    out = {
        "cp": H.cpmass(),
        "cv": H.cvmass(),
        "a": H.speed_sound(),
        "kappa": H.isothermal_compressibility(),
        "beta": H.isobaric_expansion_coefficient(),
        "ddph": H.first_partial_deriv(CP.iDmass, CP.iP, CP.iHmass),
        "ddhp": H.first_partial_deriv(CP.iDmass, CP.iHmass, CP.iP),
    }
    if transport:
        out["eta"] = H.viscosity()
        out["lambda"] = H.conductivity()
    return out


def ref_sat_p(H, p_Pa):
    """Plain HEOS saturation state at p, as setSat_p does."""
    out = {}
    H.unspecify_phase()
    H.update(CP.PQ_INPUTS, p_Pa, 0)
    out["T_sat"] = H.T()
    out["dTp"] = H.first_saturation_deriv(CP.iT, CP.iP)
    out["rho_l"] = H.rhomass()
    out["h_l"] = H.hmass()
    out["ddldp"] = H.first_saturation_deriv(CP.iDmass, CP.iP)
    out["dhldp"] = H.first_saturation_deriv(CP.iHmass, CP.iP)
    H.update(CP.PQ_INPUTS, p_Pa, 1)
    out["rho_v"] = H.rhomass()
    out["h_v"] = H.hmass()
    out["ddvdp"] = H.first_saturation_deriv(CP.iDmass, CP.iP)
    out["dhvdp"] = H.first_saturation_deriv(CP.iHmass, CP.iP)
    return out


# ---------------------------------------------------------------------------
# Variant A: SVDSBTL plus finite differences (as in commit 5966387)
# ---------------------------------------------------------------------------

def _eval_table_ph(S, h, p):
    try:
        S.update(CP.HmassP_INPUTS, h, p)
        rho, T = S.rhomass(), S.T()
        ok = math.isfinite(rho) and math.isfinite(T)
        return ok, rho, T, is_twophase(S.phase())
    except Exception:
        return False, NAN, NAN, False


def _fd(f0, fp, fm, dx, ok_p, ok_m):
    if ok_p and ok_m:
        return (fp - fm) / (2 * dx)
    if ok_p:
        return (fp - f0) / dx
    if ok_m:
        return (f0 - fm) / dx
    return NAN


def _props_ph_A(S, h, p, transport=True):
    ok, rho0, T0, tp0 = _eval_table_ph(S, h, p)
    if not ok:
        raise ValueError("table failed")
    out = {"T": T0, "rho": rho0, "twophase": tp0}
    if not tp0:
        out["a"] = S.speed_sound()
        if transport:
            out["eta"] = S.viscosity()
            out["lambda"] = S.conductivity()
    dh = REL_STEP * (abs(h) + H_SCALE_J_KG)
    dp = REL_STEP * p
    okhp, rhp, Thp, tphp = _eval_table_ph(S, h + dh, p)
    okhm, rhm, Thm, tphm = _eval_table_ph(S, h - dh, p)
    okpp, rpp, Tpp, tppp = _eval_table_ph(S, h, p + dp)
    okpm, rpm, Tpm, tppm = _eval_table_ph(S, h, p - dp)
    okhp = okhp and tphp == tp0
    okhm = okhm and tphm == tp0
    okpp = okpp and tppp == tp0
    okpm = okpm and tppm == tp0
    rho_h = _fd(rho0, rhp, rhm, dh, okhp, okhm)
    rho_p = _fd(rho0, rpp, rpm, dp, okpp, okpm)
    out["ddhp"] = rho_h
    out["ddph"] = rho_p
    if not tp0:
        T_h = _fd(T0, Thp, Thm, dh, okhp, okhm)
        T_p = _fd(T0, Tpp, Tpm, dp, okpp, okpm)
        cp = 1.0 / T_h
        beta = -rho_h / (rho0 * T_h)
        kappa = (rho_p - rho_h * T_p / T_h) / rho0
        out.update(cp=cp, beta=beta, kappa=kappa,
                   cv=cp - T0 * beta * beta / (rho0 * kappa))
    return out


def variant_A_ph(S, p_Pa, h_J_kg, transport=True):
    S.update(CP.HmassP_INPUTS, h_J_kg, p_Pa)
    T, rho, phase = S.T(), S.rhomass(), S.phase()
    twophase = is_twophase(phase)
    x = S.Q() if twophase else -1.0
    base = _props_ph_A(S, h_J_kg, p_Pa, transport)
    out = {"T": T, "rho": rho, "p": p_Pa, "h": h_J_kg, "phase": phase,
           "ddph": base["ddph"], "ddhp": base["ddhp"]}
    if not twophase:
        if "cp" not in base:
            # The 5966387 code stops the simulation here
            raise ValueError("A: single-phase properties not available")
        for k in ("cp", "cv", "kappa", "beta", "a", "eta", "lambda"):
            if k in base:
                out[k] = base[k]
    else:
        out["x"] = x
        S.update(CP.PQ_INPUTS, p_Pa, 0)
        h_l = S.hmass()
        S.update(CP.PQ_INPUTS, p_Pa, 1)
        h_v = S.hmass()
        pl = _props_ph_A(S, h_l - REL_STEP * (abs(h_l) + H_SCALE_J_KG), p_Pa, transport)
        pv = _props_ph_A(S, h_v + REL_STEP * (abs(h_v) + H_SCALE_J_KG), p_Pa, transport)
        for k in ("cp", "cv", "a"):
            out[k] = pl.get(k, NAN) + x * (pv.get(k, NAN) - pl.get(k, NAN))
    return out


def variant_A_sat_p(S, p_Pa):
    dp = REL_STEP * p_Pa
    vals = []
    for pp in (p_Pa, p_Pa + dp, p_Pa - dp):
        try:
            S.update(CP.PQ_INPUTS, pp, 0)
            T, rl, hl = S.T(), S.rhomass(), S.hmass()
            S.update(CP.PQ_INPUTS, pp, 1)
            rv, hv = S.rhomass(), S.hmass()
            vals.append((True, T, rl, hl, rv, hv))
        except Exception:
            vals.append((False, NAN, NAN, NAN, NAN, NAN))
    (ok0, T0, rl0, hl0, rv0, hv0), p1, m1 = vals
    out = {"T_sat": T0, "rho_l": rl0, "h_l": hl0, "rho_v": rv0, "h_v": hv0}
    names = ["dTp", "ddldp", "dhldp", "ddvdp", "dhvdp"]
    for i, name in enumerate(names):
        j = [1, 2, 3, 4, 5][i]
        out[name] = _fd(vals[0][j], p1[j], m1[j], dp, p1[0], m1[0])
    return out


# ---------------------------------------------------------------------------
# Variants B and C: the table inverts, HEOS evaluates
# ---------------------------------------------------------------------------

def heos_at_Trho(H, T_K, rho_kg_m3, phase):
    """Evaluate HEOS directly at (T, rho) with the phase imposed.

    A Helmholtz equation of state is explicit in (T, rho), so with the
    phase imposed CoolProp does no iteration and no phase check here.
    """
    H.specify_phase(phase)
    H.update(CP.DmassT_INPUTS, rho_kg_m3, T_K)


def newton_step_ph(H, p_in_Pa, h_in_J_kg, T_K, rho_kg_m3, phase):
    """One Newton step on (T, rho) so that p(T, rho) = p_in and h(T, rho) = h_in.

    H must hold the state at (T_K, rho_kg_m3). The residuals are
        r_p = p(T, rho) - p_in
        r_h = h(T, rho) - h_in
    and the 2x2 Jacobian holds the analytical partial derivatives of the
    Helmholtz equation of state. Returns the corrected (T, rho) and
    leaves H at that corrected state.
    """
    r_p = H.p() - p_in_Pa
    r_h = H.hmass() - h_in_J_kg
    dpdT = H.first_partial_deriv(CP.iP, CP.iT, CP.iDmass)
    dpdrho = H.first_partial_deriv(CP.iP, CP.iDmass, CP.iT)
    dhdT = H.first_partial_deriv(CP.iHmass, CP.iT, CP.iDmass)
    dhdrho = H.first_partial_deriv(CP.iHmass, CP.iDmass, CP.iT)
    det = dpdT * dhdrho - dpdrho * dhdT
    dT = (r_p * dhdrho - r_h * dpdrho) / det
    drho = (dpdT * r_h - dhdT * r_p) / det
    T_new = T_K - dT
    rho_new = rho_kg_m3 - drho
    heos_at_Trho(H, T_new, rho_new, phase)
    return T_new, rho_new


def converged_ph(H, p_in_Pa, h_in_J_kg, tol=1e-9):
    """True if the HEOS state H matches the inputs (p, h).

    The pressure residual is measured by the relative density change it
    implies, kappa |p - p_in| = |d rho| / rho, because p computed from
    (T, rho) of a stiff liquid carries a round-off error that can be
    larger than 1e-9 p at low pressure (water at 1 to 10 kPa).
    """
    ok_p = abs(H.p() - p_in_Pa) * H.isothermal_compressibility() < tol
    ok_h = abs(H.hmass() - h_in_J_kg) < tol * (abs(h_in_J_kg) + H_SCALE_J_KG)
    return ok_p and ok_h


def sat_side_derivs(H, dTp):
    """Derivatives of rho and h along the saturation line, d/dp at one side.

    Along the saturation line T = T_sat(p), so
        d rho/dp|sat = (d rho/dp)_T + (d rho/dT)_p * dT_sat/dp
    and the same for h.
    """
    drho = (H.first_partial_deriv(CP.iDmass, CP.iP, CP.iT)
            + H.first_partial_deriv(CP.iDmass, CP.iT, CP.iP) * dTp)
    dh = (H.first_partial_deriv(CP.iHmass, CP.iP, CP.iT)
          + H.first_partial_deriv(CP.iHmass, CP.iT, CP.iP) * dTp)
    return drho, dh


def newton_step_sat(H, p_in_Pa, T_K, rho_l, rho_v):
    """One Newton step on the phase equilibrium at a given pressure.

    Unknowns: T, rho_l, rho_v. Equations:
        p(T, rho_l) - p_in = 0
        p(T, rho_v) - p_in = 0
        g(T, rho_l) - g(T, rho_v) = 0   (equal Gibbs energy)
    with dg = v dp - s dT, so at constant rho
        (dg/dT)_rho = v (dp/dT)_rho - s,   (dg/drho)_T = v (dp/drho)_T.
    """
    rows = []
    for rho, ph in ((rho_l, CP.iphase_liquid), (rho_v, CP.iphase_gas)):
        heos_at_Trho(H, T_K, rho, ph)
        p = H.p()
        g = H.gibbsmass()
        dpdT = H.first_partial_deriv(CP.iP, CP.iT, CP.iDmass)
        dpdrho = H.first_partial_deriv(CP.iP, CP.iDmass, CP.iT)
        v = 1.0 / rho
        rows.append((p, g, dpdT, dpdrho, v * dpdT - H.smass(), v * dpdrho))
    (pl, gl, pTl, prl, gTl, grl), (pv, gv, pTv, prv, gTv, grv) = rows
    # Solve J * d = r for d = (dT, drho_l, drho_v), 3x3 by Cramer's rule
    J = [[pTl, prl, 0.0], [pTv, 0.0, prv], [gTl - gTv, grl, -grv]]
    r = [pl - p_in_Pa, pv - p_in_Pa, gl - gv]

    def det3(m):
        return (m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1])
                - m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0])
                + m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]))
    D = det3(J)
    d = []
    for k in range(3):
        Jk = [row[:] for row in J]
        for i in range(3):
            Jk[i][k] = r[i]
        d.append(det3(Jk) / D)
    return T_K - d[0], rho_l - d[1], rho_v - d[2]


def sat_BC(S, Hl, Hv, p_Pa, newton):
    """Saturation state at p for variants B (newton=False) and C (True).

    Hl and Hv are two HEOS states, left at the liquid and vapour side.
    """
    S.update(CP.PQ_INPUTS, p_Pa, 0)
    T = S.T()
    rho_l = S.rhomass()
    S.update(CP.PQ_INPUTS, p_Pa, 1)
    rho_v = S.rhomass()
    if newton:
        T, rho_l, rho_v = newton_step_sat(Hl, p_Pa, T, rho_l, rho_v)
    heos_at_Trho(Hl, T, rho_l, CP.iphase_liquid)
    heos_at_Trho(Hv, T, rho_v, CP.iphase_gas)
    h_l, h_v = Hl.hmass(), Hv.hmass()
    # Clausius-Clapeyron: dT_sat/dp = T (v_v - v_l) / (h_v - h_l)
    dTp = T * (1.0 / rho_v - 1.0 / rho_l) / (h_v - h_l)
    ddldp, dhldp = sat_side_derivs(Hl, dTp)
    ddvdp, dhvdp = sat_side_derivs(Hv, dTp)
    return {"T_sat": T, "rho_l": rho_l, "rho_v": rho_v, "h_l": h_l, "h_v": h_v,
            "dTp": dTp, "ddldp": ddldp, "dhldp": dhldp, "ddvdp": ddvdp,
            "dhvdp": dhvdp, "p_l": Hl.p(), "p_v": Hv.p()}


def single_phase_label(H, T_K, rho_kg_m3, p_Pa):
    """CoolProp phase label for a single-phase state.

    SVDSBTL only reports whether a point is two-phase, its phase() is
    iphase_not_imposed for single-phase points. Any single-phase label
    makes HEOS skip its saturation check at (T, rho), so a simple rule
    is enough: above the critical temperature or pressure the state is
    supercritical, below both it is liquid if denser than the critical
    density and vapour otherwise.
    """
    T_c, p_c = H.T_critical(), H.p_critical()
    if T_K >= T_c and p_Pa >= p_c:
        return CP.iphase_supercritical
    if T_K >= T_c:
        return CP.iphase_supercritical_gas
    if p_Pa >= p_c:
        return CP.iphase_supercritical_liquid
    return CP.iphase_liquid if rho_kg_m3 > H.rhomass_critical() else CP.iphase_gas


def variant_BC_ph(S, H, Hl, Hv, p_Pa, h_J_kg, newton, transport=True):
    S.update(CP.HmassP_INPUTS, h_J_kg, p_Pa)
    phase = S.phase()
    if not is_twophase(phase):
        T, rho = S.T(), S.rhomass()
        phase = single_phase_label(H, T, rho, p_Pa)
        heos_at_Trho(H, T, rho, phase)
        if newton:
            # newton=True: one step (variant C). newton=n (an integer > 1):
            # iterate until p and h match to 1e-9 relative, at most n steps.
            n_max = 1 if newton is True else int(newton)
            for _ in range(n_max):
                if n_max > 1 and converged_ph(H, p_Pa, h_J_kg):
                    break
                T, rho = newton_step_ph(H, p_Pa, h_J_kg, T, rho, phase)
        out = {"T": H.T(), "rho": H.rhomass(), "p": H.p(), "h": H.hmass(),
               "phase": phase}
        out.update(single_phase_outputs(H, transport))
        return out
    # Two-phase: liquid and vapour sides from HEOS, quality from h
    sat = sat_BC(S, Hl, Hv, p_Pa, newton)
    h_l, h_v = sat["h_l"], sat["h_v"]
    v_l, v_v = 1.0 / sat["rho_l"], 1.0 / sat["rho_v"]
    x = (h_J_kg - h_l) / (h_v - h_l)
    v = v_l + x * (v_v - v_l)
    rho = 1.0 / v
    # (d rho/dh)_p and (d rho/dp)_h in the two-phase region, from
    # v = v_l + x (v_v - v_l) with x = (h - h_l)/(h_v - h_l)
    dvdh = (v_v - v_l) / (h_v - h_l)
    dvldp = -sat["ddldp"] / sat["rho_l"] ** 2
    dvvdp = -sat["ddvdp"] / sat["rho_v"] ** 2
    dxdp = -((1 - x) * sat["dhldp"] + x * sat["dhvdp"]) / (h_v - h_l)
    dvdp = (1 - x) * dvldp + x * dvvdp + (v_v - v_l) * dxdp
    out = {"T": sat["T_sat"], "rho": rho, "p": p_Pa, "h": h_J_kg, "x": x,
           "phase": phase, "ddhp": -rho * rho * dvdh, "ddph": -rho * rho * dvdp}
    # extend_twophase: interpolate cp, cv, a between the saturated states
    for key, f in (("cp", "cpmass"), ("cv", "cvmass"), ("a", "speed_sound")):
        l = getattr(Hl, f)()
        vv = getattr(Hv, f)()
        out[key] = l + x * (vv - l)
    return out


def newton_step_ps(H, p_in_Pa, s_in_J_kgK, T_K, rho_kg_m3, phase):
    """One Newton step on (T, rho) for p-s inputs, as newton_step_ph with s for h."""
    r_p = H.p() - p_in_Pa
    r_s = H.smass() - s_in_J_kgK
    dpdT = H.first_partial_deriv(CP.iP, CP.iT, CP.iDmass)
    dpdrho = H.first_partial_deriv(CP.iP, CP.iDmass, CP.iT)
    dsdT = H.first_partial_deriv(CP.iSmass, CP.iT, CP.iDmass)
    dsdrho = H.first_partial_deriv(CP.iSmass, CP.iDmass, CP.iT)
    det = dpdT * dsdrho - dpdrho * dsdT
    T_new = T_K - (r_p * dsdrho - r_s * dpdrho) / det
    rho_new = rho_kg_m3 - (dpdT * r_s - dsdT * r_p) / det
    heos_at_Trho(H, T_new, rho_new, phase)
    return T_new, rho_new


def variant_BC_pT(S, H, p_Pa, T_K, newton, transport=True):
    """p-T inputs: the table gives rho, HEOS is evaluated at (T, rho).

    T is an input and stays exact. The p-T table is a poor starting
    point close to the saturation line: within about 0.01 K it can
    return the density of the wrong phase, and near the critical point
    its density error reaches tens of percent. So for C the phase comes
    from comparing T with T_sat(p) (SuperAncillary, exact to HEOS), the
    table density is moved to the right side of the saturated density
    if needed, and a 1-D Newton iteration in rho solves p(T, rho) = p_in
    to full convergence. (HEOS update_with_guesses with PT_INPUTS does
    the same, but it is barely faster than a plain HEOS p-T flash.)
    """
    S.update(CP.PT_INPUTS, p_Pa, T_K)
    rho = S.rhomass()
    if not newton:
        phase = single_phase_label(H, T_K, rho, p_Pa)
        heos_at_Trho(H, T_K, rho, phase)
    else:
        if p_Pa < H.p_critical() and T_K < H.T_critical():
            S.update(CP.PQ_INPUTS, p_Pa, 0)
            T_sat, rho_l = S.T(), S.rhomass()
            S.update(CP.PQ_INPUTS, p_Pa, 1)
            rho_v = S.rhomass()
            if T_K < T_sat:
                phase, rho = CP.iphase_liquid, max(rho, rho_l)
            else:
                phase, rho = CP.iphase_gas, min(rho, rho_v)
        else:
            phase = single_phase_label(H, T_K, rho, p_Pa)
        # 1-D Newton iteration in rho at fixed T until p matches
        for _ in range(30):
            heos_at_Trho(H, T_K, rho, phase)
            r_p = H.p() - p_Pa
            if abs(r_p) < 1e-10 * p_Pa:
                break
            rho -= r_p / H.first_partial_deriv(CP.iP, CP.iDmass, CP.iT)
    out = {"T": H.T(), "rho": H.rhomass(), "p": H.p(), "h": H.hmass()}
    out.update(single_phase_outputs(H, transport))
    return out


def variant_BC_ps(S, H, p_Pa, s_J_kgK, newton, transport=True):
    """p-s inputs, single-phase only (as variant_BC_ph)."""
    S.update(CP.PSmass_INPUTS, p_Pa, s_J_kgK)
    T, rho = S.T(), S.rhomass()
    phase = single_phase_label(H, T, rho, p_Pa)
    heos_at_Trho(H, T, rho, phase)
    if newton:
        newton_step_ps(H, p_Pa, s_J_kgK, T, rho, phase)
    out = {"T": H.T(), "rho": H.rhomass(), "p": H.p(), "s": H.smass()}
    out.update(single_phase_outputs(H, transport))
    return out


def accuracy_pT_ps(fluid):
    """Single-phase p-T and p-s inputs, variants B and C.

    Returns err[pair][variant][group][prop] with group "near" for
    points within 0.1 K of the saturation line and "far" otherwise,
    and the consistency with the inputs.
    """
    S = CP.AbstractState("SVDSBTL&HEOS", fluid)
    H = CP.AbstractState("HEOS", fluid)
    E = CP.AbstractState("HEOS", fluid)
    err = {pair: {v: {} for v in "BC"} for pair in ("pT", "ps")}
    cons = {pair: {v: {} for v in "BC"} for pair in ("pT", "ps")}
    fails = {pair: {v: 0 for v in "BC"} for pair in ("pT", "ps")}
    for bucket, p, h in test_points(H):
        if bucket.startswith("two"):
            continue
        group = "far"
        for tag in ("=0.001 K", "=0.01 K", "=0.1 K"):
            if bucket.endswith(tag):
                group = "near"
        ref = ref_ph(H, p, h)
        H.update(CP.HmassP_INPUTS, h, p)
        T, s_in = H.T(), H.smass()
        for pair in ("pT", "ps"):
            for v in "BC":
                try:
                    if pair == "pT":
                        res = variant_BC_pT(S, E, p, T, v == "C")
                        c = abs(res["p"] - p) / p
                    else:
                        res = variant_BC_ps(S, E, p, s_in, v == "C")
                        c = max(abs(res["p"] - p) / p, abs(res["s"] - s_in) / abs(s_in))
                except Exception:
                    fails[pair][v] += 1
                    continue
                b = err[pair][v].setdefault(group, {})
                for k in PROPS:
                    e = rel_err(res.get(k), ref.get(k))
                    if math.isfinite(e):
                        b[k] = max(b.get(k, 0.0), e)
                cons[pair][v][group] = max(cons[pair][v].get(group, 0.0), c)
    return err, cons, fails


def print_pT_ps(fluid, err, cons, fails):
    print("\nSingle-phase p-T and p-s inputs, maximum relative error, B / C. "
          "For p-T, C is a converged Newton iteration from the table guess. "
          "near: within 0.1 K of saturation, far: the other single-phase "
          "points. Failed calls p-T: B %d, C %d; p-s: B %d, C %d.\n"
          % (fails["pT"]["B"], fails["pT"]["C"], fails["ps"]["B"], fails["ps"]["C"]))
    print("| inputs | region | " + " | ".join(PROPS) + " | input mismatch |")
    print("|---" * (len(PROPS) + 3) + "|")
    for pair in ("pT", "ps"):
        for group in ("near", "far"):
            cells = [" / ".join(fmt(err[pair][v].get(group, {}).get(k)) for v in "BC")
                     for k in PROPS]
            m = " / ".join(fmt(cons[pair][v].get(group)) for v in "BC")
            print("| %s | %s | %s | %s |" % (pair, group, " | ".join(cells), m))


# ---------------------------------------------------------------------------
# Test points
# ---------------------------------------------------------------------------

def test_points(H):
    """Points as (bucket, p, h) on a grid of reduced pressures.

    Buckets give the distance from the saturation line: subcooling or
    superheat dT in K, or the vapour quality x.
    """
    pc = H.p_critical()
    Tc = H.T_critical()
    p_min = 1.5 * H.trivial_keyed_output(CP.iP_triple)
    T_min = H.Ttriple() + 2.0
    pts = []
    for pr in (0.01, 0.05, 0.2, 0.5, 0.8, 0.95, 0.99):
        p = pr * pc
        if p < p_min:
            continue
        H.unspecify_phase()
        H.update(CP.PQ_INPUTS, p, 0)
        Ts = H.T()
        for dT in (0.001, 0.01, 0.1, 1.0, 10.0):
            if Ts - dT > T_min:
                H.specify_phase(CP.iphase_liquid)
                H.update(CP.PT_INPUTS, p, Ts - dT)
                pts.append(("liquid dT_sc=%g K" % dT, p, H.hmass()))
            H.specify_phase(CP.iphase_gas)
            H.update(CP.PT_INPUTS, p, Ts + dT)
            pts.append(("vapour dT_sh=%g K" % dT, p, H.hmass()))
        H.unspecify_phase()
        for x in (0.001, 0.01, 0.5, 0.99, 0.999):
            H.update(CP.PQ_INPUTS, p, x)
            pts.append(("two-phase x=%g" % x, p, H.hmass()))
    H.unspecify_phase()
    for pr in (1.2, 2.0, 4.0):
        for Tr in (0.8, 0.95, 1.05, 1.2, 2.0):
            if Tr * Tc > H.Tmax():
                continue
            H.update(CP.PT_INPUTS, pr * pc, Tr * Tc)
            pts.append(("supercritical", pr * pc, H.hmass()))
    return pts


BUCKET_ORDER = (["liquid dT_sc=%g K" % d for d in (0.001, 0.01, 0.1, 1.0, 10.0)]
                + ["vapour dT_sh=%g K" % d for d in (0.001, 0.01, 0.1, 1.0, 10.0)]
                + ["two-phase x=%g" % x for x in (0.001, 0.01, 0.5, 0.99, 0.999)]
                + ["supercritical"])

PROPS = ["rho", "T", "cp", "cv", "a", "kappa", "beta", "ddph", "ddhp"]
SAT_PROPS = ["T_sat", "rho_l", "rho_v", "h_l", "h_v",
             "dTp", "ddldp", "dhldp", "ddvdp", "dhvdp"]


def fmt(e):
    if e is None or not math.isfinite(e):
        return "-"
    if e == 0.0:
        return "0"
    return "%.0e" % e


# ---------------------------------------------------------------------------
# Accuracy study
# ---------------------------------------------------------------------------

def accuracy(fluid):
    S = CP.AbstractState("SVDSBTL&HEOS", fluid)
    H = CP.AbstractState("HEOS", fluid)       # reference
    E = CP.AbstractState("HEOS", fluid)       # evaluation state of B and C
    Hl = CP.AbstractState("HEOS", fluid)
    Hv = CP.AbstractState("HEOS", fluid)
    pts = test_points(H)

    # The existing table options, for comparison
    tables = {"TTSE": CP.AbstractState("TTSE&HEOS", fluid),
              "BICUBIC": CP.AbstractState("BICUBIC&HEOS", fluid)}

    # err[variant][bucket][prop] = max relative error over the pressures
    err = {v: {} for v in ["A", "B", "C", "TTSE", "BICUBIC"]}
    fails = {v: 0 for v in err}
    phase_mismatch = 0
    cons = {"B": {}, "C": {}}   # consistency: max |p-p_in|/p_in and |h-h_in|
    for bucket, p, h in pts:
        ref = ref_ph(H, p, h)
        S.update(CP.HmassP_INPUTS, h, p)
        if is_twophase(S.phase()) != is_twophase(ref["phase"]):
            phase_mismatch += 1
        for v in err:
            try:
                if v in tables:
                    res = ref_ph(tables[v], p, h)
                elif v == "A":
                    res = variant_A_ph(S, p, h)
                else:
                    res = variant_BC_ph(S, E, Hl, Hv, p, h, newton=(v == "C"))
            except Exception:
                fails[v] += 1
                continue
            b = err[v].setdefault(bucket, {})
            for k in PROPS:
                if k in ref:
                    e = rel_err(res.get(k), ref[k])
                    if math.isfinite(e):
                        b[k] = max(b.get(k, 0.0), e)
            if v in cons:
                c = cons[v].setdefault(bucket, [0.0, 0.0])
                c[0] = max(c[0], abs(res["p"] - p) / p)
                c[1] = max(c[1], abs(res["h"] - h))

    # Saturation properties
    sat_err = {v: {} for v in "ABC"}
    sat_cons = {"B": 0.0, "C": 0.0}
    pc = H.p_critical()
    p_min = 1.5 * H.trivial_keyed_output(CP.iP_triple)
    for pr in (0.01, 0.05, 0.2, 0.5, 0.8, 0.95, 0.99):
        p = pr * pc
        if p < p_min:
            continue
        ref = ref_sat_p(H, p)
        for v in "ABC":
            res = (variant_A_sat_p(S, p) if v == "A"
                   else sat_BC(S, Hl, Hv, p, newton=(v == "C")))
            for k in SAT_PROPS:
                e = rel_err(res.get(k), ref[k])
                if math.isfinite(e):
                    sat_err[v][k] = max(sat_err[v].get(k, 0.0), e)
            if v in sat_cons:
                sat_cons[v] = max(sat_cons[v], abs(res["p_l"] - p) / p,
                                  abs(res["p_v"] - p) / p)
    return dict(err=err, fails=fails, mismatch=phase_mismatch, npts=len(pts),
                cons=cons, sat_err=sat_err, sat_cons=sat_cons)


def print_accuracy(fluid, r):
    print("\n### %s\n" % fluid)
    print("%d p-h test points, %d with a different phase (one- or two-phase) "
          "in the table than in HEOS. Failed calls: A %d, B %d, C %d, TTSE %d, BICUBIC %d.\n"
          % (r["npts"], r["mismatch"], r["fails"]["A"], r["fails"]["B"], r["fails"]["C"],
             r["fails"]["TTSE"], r["fails"]["BICUBIC"]))
    print("Maximum relative error against plain HEOS, p-h inputs, "
          "cells are A / B / C:\n")
    print("| region | " + " | ".join(PROPS) + " |")
    print("|---" * (len(PROPS) + 1) + "|")
    for bucket in BUCKET_ORDER:
        if not any(bucket in r["err"][v] for v in "ABC"):
            continue
        cells = []
        for k in PROPS:
            cells.append(" / ".join(fmt(r["err"][v].get(bucket, {}).get(k)) for v in "ABC"))
        print("| %s | %s |" % (bucket, " | ".join(cells)))
    print("\nConsistency with the inputs, max |p - p_in|/p_in and max |h - h_in| "
          "in J/kg, B / C:\n")
    print("| region | p rel. | h J/kg |")
    print("|---|---|---|")
    for bucket in BUCKET_ORDER:
        if bucket not in r["cons"]["B"]:
            continue
        b = r["cons"]["B"][bucket]
        c = r["cons"]["C"].get(bucket, [NAN, NAN])
        print("| %s | %s / %s | %.2g / %.2g |" % (bucket, fmt(b[0]), fmt(c[0]), b[1], c[1]))
    print("\nSaturation properties at p, maximum relative error, A / B / C:\n")
    print("| " + " | ".join(SAT_PROPS) + " |")
    print("|---" * len(SAT_PROPS) + "|")
    print("| " + " | ".join(" / ".join(fmt(r["sat_err"][v].get(k)) for v in "ABC")
                            for k in SAT_PROPS) + " |")
    print("\nSaturation pressure consistency, max |p(T_sat, rho) - p|/p: B %s, C %s"
          % (fmt(r["sat_cons"]["B"]), fmt(r["sat_cons"]["C"])))


# ---------------------------------------------------------------------------
# Python timing (includes Python call overhead, see the C++ benchmark)
# ---------------------------------------------------------------------------

def timing(fluid, transport, n_rep=3):
    S = CP.AbstractState("SVDSBTL&HEOS", fluid)
    E = CP.AbstractState("HEOS", fluid)
    Hl = CP.AbstractState("HEOS", fluid)
    Hv = CP.AbstractState("HEOS", fluid)
    H = CP.AbstractState("HEOS", fluid)
    pts = test_points(H)
    single = [(p, h) for b, p, h in pts if not b.startswith("two")]
    twoph = [(p, h) for b, p, h in pts if b.startswith("two")]
    res = {}
    for name in ("HEOS", "TTSE", "BICUBIC"):
        st = H if name == "HEOS" else CP.AbstractState(name + "&HEOS", fluid)

        def f(p, h, st=st):
            st.update(CP.HmassP_INPUTS, h, p)
            if is_twophase(st.phase()):
                st.first_two_phase_deriv(CP.iDmass, CP.iP, CP.iHmass)
                st.first_two_phase_deriv(CP.iDmass, CP.iHmass, CP.iP)
                st.saturated_liquid_keyed_output(CP.iCpmass)
                st.saturated_vapor_keyed_output(CP.iCpmass)
            else:
                single_phase_outputs(st, transport)
        res[name] = f
    res["A"] = lambda p, h: variant_A_ph(S, p, h, transport)
    res["B"] = lambda p, h: variant_BC_ph(S, E, Hl, Hv, p, h, False, transport)
    res["C"] = lambda p, h: variant_BC_ph(S, E, Hl, Hv, p, h, True, transport)
    out = {}
    for name, f in res.items():
        for label, ps in (("single", single), ("twophase", twoph)):
            best = float("inf")
            for _ in range(n_rep):
                t0 = time.perf_counter()
                for p, h in ps:
                    try:
                        f(p, h)
                    except Exception:
                        pass
                best = min(best, (time.perf_counter() - t0) / len(ps))
            out[(name, label)] = best * 1e6
    return out


def random_check(fluid, n=2000, seed=1, newton=True):
    """Random single-phase p-h points close to the saturation lines and
    around the critical point, variant C only. Returns the worst relative
    error over all PROPS and the worst p and h mismatch after the Newton
    step, plus the number of failed calls.
    """
    import random
    rnd = random.Random(seed)
    S = CP.AbstractState("SVDSBTL&HEOS", fluid)
    H = CP.AbstractState("HEOS", fluid)
    E = CP.AbstractState("HEOS", fluid)
    Hl = CP.AbstractState("HEOS", fluid)
    Hv = CP.AbstractState("HEOS", fluid)
    pc, Tc = H.p_critical(), H.T_critical()
    p_lo = 1.5 * H.trivial_keyed_output(CP.iP_triple)
    worst = {"err": 0.0, "prop": "", "p_rel": 0.0, "h_J_kg": 0.0, "where": ""}
    fails = 0
    for i in range(n):
        try:
            if i % 4 == 3:
                # around the critical point, p_r 0.9 to 1.3, T_r 0.97 to 1.1
                p = pc * rnd.uniform(0.9, 1.3)
                T = Tc * rnd.uniform(0.97, 1.1)
                H.unspecify_phase()
                H.update(CP.PT_INPUTS, p, T)
                if is_twophase(H.phase()):
                    continue
            else:
                p = math.exp(rnd.uniform(math.log(p_lo), math.log(0.999 * pc)))
                H.unspecify_phase()
                H.update(CP.PQ_INPUTS, p, 0)
                Ts = H.T()
                dT = 10 ** rnd.uniform(-4, 1.5)
                if i % 2:
                    H.specify_phase(CP.iphase_gas)
                    H.update(CP.PT_INPUTS, p, Ts + dT)
                else:
                    if Ts - dT < H.Ttriple() + 1:
                        continue
                    H.specify_phase(CP.iphase_liquid)
                    H.update(CP.PT_INPUTS, p, Ts - dT)
            h = H.hmass()
            ref = ref_ph(H, p, h)
        except Exception:
            continue
        try:
            res = variant_BC_ph(S, E, Hl, Hv, p, h, newton=newton)
        except Exception:
            fails += 1
            continue
        for k in PROPS:
            e = rel_err(res.get(k), ref.get(k))
            if math.isfinite(e) and e > worst["err"]:
                worst.update(err=e, prop=k, where="p_r=%.4f T=%.4f K" % (p / pc, ref["T"]))
        worst["p_rel"] = max(worst["p_rel"], abs(res["p"] - p) / p)
        worst["h_J_kg"] = max(worst["h_J_kg"], abs(res["h"] - h))
    return worst, fails


GROUPS = [
    ("liquid, dT_sc <= 0.01 K", ["liquid dT_sc=0.001 K", "liquid dT_sc=0.01 K"]),
    ("liquid, dT_sc = 0.1 K", ["liquid dT_sc=0.1 K"]),
    ("liquid, dT_sc >= 1 K", ["liquid dT_sc=1 K", "liquid dT_sc=10 K"]),
    ("vapour, dT_sh <= 0.01 K", ["vapour dT_sh=0.001 K", "vapour dT_sh=0.01 K"]),
    ("vapour, dT_sh = 0.1 K", ["vapour dT_sh=0.1 K"]),
    ("vapour, dT_sh >= 1 K", ["vapour dT_sh=1 K", "vapour dT_sh=10 K"]),
    ("two-phase, 0.001 <= x <= 0.999", ["two-phase x=%g" % x for x in (0.001, 0.01, 0.5, 0.99, 0.999)]),
    ("supercritical", ["supercritical"]),
]
DERIV_PROPS = ["cp", "cv", "a", "kappa", "beta", "ddph", "ddhp"]


def print_summary(results):
    """Condensed table: worst case over the three fluids, per region group."""
    def worst(v, buckets, props):
        w = 0.0
        for r in results.values():
            for b in buckets:
                for k in props:
                    e = r["err"][v].get(b, {}).get(k)
                    if e is not None and math.isfinite(e):
                        w = max(w, e)
        return w
    print("\n## Summary, p-h inputs, worst case over %s\n" % ", ".join(results))
    print("Maximum relative error against plain HEOS. State: rho and T. "
          "Derivatives: cp, cv, speed of sound, kappa, beta, ddph, ddhp. "
          "Input mismatch of B: max |h(T, rho) - h_in| in J/kg (C: below 0.2 J/kg).\n")
    print("| region | state A | state B | state C | derivatives A | derivatives B | derivatives C | h mismatch B "
          "| derivatives TTSE | derivatives BICUBIC |")
    print("|---|---|---|---|---|---|---|---|---|---|")
    for name, buckets in GROUPS:
        cells = [fmt(worst(v, buckets, ["rho", "T"])) for v in "ABC"]
        cells += [fmt(worst(v, buckets, DERIV_PROPS)) for v in "ABC"]
        hm = max(r["cons"]["B"].get(b, [0, 0])[1] for r in results.values() for b in buckets)
        cells.append("%.2g" % hm)
        cells += [fmt(worst(v, buckets, DERIV_PROPS)) for v in ("TTSE", "BICUBIC")]
        print("| %s | %s |" % (name, " | ".join(cells)))
    print("\nSaturation properties (setSat_p), worst over all props and fluids: "
          "A %s (T_sat, rho, h exact from the table, derivatives by finite "
          "differences), B %s, C %s." % tuple(
              fmt(max(max(r["sat_err"][v].values()) for r in results.values())) for v in "ABC"))


def main():
    CP.set_config_int(CP.SVDSBTL_SAMPLING_THREADS, 0)
    print("CoolProp %s\n" % CP.get_global_param_string("version"))
    results = {}
    for fluid in FLUIDS:
        results[fluid] = accuracy(fluid)
    print_summary(results)
    print("\n## Details per fluid")
    for fluid in FLUIDS:
        print_accuracy(fluid, results[fluid])
        print_pT_ps(fluid, *accuracy_pT_ps(fluid))
    if "--random" in sys.argv:
        print("\n### Random points, variant C\n")
        print("| fluid | Newton | worst rel. error | property | where | max p mismatch | max h mismatch J/kg | failed |")
        print("|---|---|---|---|---|---|---|---|")
        for fluid in FLUIDS:
            for newton, label in ((True, "one step"), (5, "converged, max 5")):
                w, nf = random_check(fluid, newton=newton)
                print("| %s | %s | %s | %s | %s | %s | %.2g | %d |" % (
                    fluid, label, fmt(w["err"]), w["prop"], w["where"], fmt(w["p_rel"]), w["h_J_kg"], nf))
    if "--timing" in sys.argv:
        print("\n### Python timing, microseconds per p-h call (with Python overhead)\n")
        names = ["HEOS", "TTSE", "BICUBIC", "A", "B", "C"]
        print("| fluid | transport | region | " + " | ".join(names) + " |")
        print("|---" * (len(names) + 3) + "|")
        for fluid in FLUIDS:
            for tr in (False, True):
                t = timing(fluid, tr)
                for label in ("single", "twophase"):
                    print("| %s | %s | %s | %s |" % (
                        fluid, "yes" if tr else "no", label,
                        " | ".join("%.1f" % t[(n, label)] for n in names)))


if __name__ == "__main__":
    main()
