/*
 * Timing benchmark for the SVDSBTL variants of svdsbtl_prototype.py.
 *
 * It measures the time per p-h call for everything ExternalMedia needs
 * in postStateChange (state, cp, cv, speed of sound, kappa, beta, ddph,
 * ddhp and optionally viscosity and conductivity):
 *
 *   HEOS     plain HEOS p-h flash
 *   TTSE     TTSE&HEOS tables
 *   BICUBIC  BICUBIC&HEOS tables
 *   A        SVDSBTL plus finite differences (commit 5966387)
 *   B        SVDSBTL gives T and rho, HEOS evaluated at (T, rho)
 *   C        like B, plus one Newton step on HEOS in (p, h)
 *   C conv.  like C, Newton steps until the inputs match (at most 5)
 *
 * and the time per setSat_p call (plain HEOS against variant B).
 *
 * Build against a static CoolProp 8.0.0 library, for example:
 *
 *   g++ -O2 -std=c++17 svdsbtl_benchmark.cpp -I<CoolProp>/include \
 *       -I<CoolProp>/externals/fmtlib/include <build>/libCoolProp.a \
 *       -ldl -lpthread -o svdsbtl_benchmark
 *
 * The SVDSBTL tables must exist in ~/.CoolProp/SVDTables (run the Python
 * prototype first, or wait for the first build).
 */

#include "AbstractState.h"
#include "CoolProp.h"
#include "DataStructures.h"
#include "Configuration.h"

#include <chrono>
#include <cmath>
#include <algorithm>
#include <cstdio>
#include <memory>
#include <string>
#include <vector>

using CoolProp::AbstractState;
using State = std::shared_ptr<AbstractState>;

struct Out
{
    double T, rho, p, h, cp, cv, a, kappa, beta, ddph, ddhp, eta, lambda;
};

static State make(const std::string& backend, const std::string& fluid) {
    return State(AbstractState::factory(backend, fluid));
}

// Single-phase outputs from a state that is already updated
static void single_phase(AbstractState& S, Out& o, bool transport) {
    o.T = S.T();
    o.rho = S.rhomass();
    o.p = S.p();
    o.h = S.hmass();
    o.cp = S.cpmass();
    o.cv = S.cvmass();
    o.a = S.speed_sound();
    o.kappa = S.isothermal_compressibility();
    o.beta = S.isobaric_expansion_coefficient();
    o.ddph = S.first_partial_deriv(CoolProp::iDmass, CoolProp::iP, CoolProp::iHmass);
    o.ddhp = S.first_partial_deriv(CoolProp::iDmass, CoolProp::iHmass, CoolProp::iP);
    if (transport) {
        o.eta = S.viscosity();
        o.lambda = S.conductivity();
    }
}

// Two-phase outputs of a table or HEOS state (extend_twophase behaviour)
static void two_phase(AbstractState& S, Out& o) {
    const double x = S.Q();
    o.T = S.T();
    o.rho = S.rhomass();
    o.ddph = S.first_two_phase_deriv(CoolProp::iDmass, CoolProp::iP, CoolProp::iHmass);
    o.ddhp = S.first_two_phase_deriv(CoolProp::iDmass, CoolProp::iHmass, CoolProp::iP);
    const double cpl = S.saturated_liquid_keyed_output(CoolProp::iCpmass);
    const double cpv = S.saturated_vapor_keyed_output(CoolProp::iCpmass);
    const double cvl = S.saturated_liquid_keyed_output(CoolProp::iCvmass);
    const double cvv = S.saturated_vapor_keyed_output(CoolProp::iCvmass);
    o.cp = cpl + x * (cpv - cpl);
    o.cv = cvl + x * (cvv - cvl);
}

// Plain backend (HEOS, TTSE, BICUBIC): p-h flash
static void plain_ph(AbstractState& S, double p, double h, Out& o, bool transport) {
    S.update(CoolProp::HmassP_INPUTS, h, p);
    if (S.phase() == CoolProp::iphase_twophase)
        two_phase(S, o);
    else
        single_phase(S, o, transport);
}

// ---------------------------------------------------------------- variant A
static const double REL_STEP = 1e-5;
static const double H_SCALE_J_KG = 1e5;

static bool eval_table(AbstractState& S, double h, double p, double& rho, double& T, bool& tp) {
    try {
        S.update(CoolProp::HmassP_INPUTS, h, p);
        rho = S.rhomass();
        T = S.T();
        tp = (S.phase() == CoolProp::iphase_twophase);
    } catch (...) {
        return false;
    }
    return std::isfinite(rho) && std::isfinite(T);
}

static double fd(double f0, double fp, double fm, double dx, bool okp, bool okm) {
    if (okp && okm) return (fp - fm) / (2 * dx);
    if (okp) return (fp - f0) / dx;
    if (okm) return (f0 - fm) / dx;
    return NAN;
}

static void props_A(AbstractState& S, double h, double p, Out& o, bool transport) {
    double rho0, T0;
    bool tp0;
    eval_table(S, h, p, rho0, T0, tp0);
    if (!tp0) {
        o.a = S.speed_sound();
        if (transport) {
            o.eta = S.viscosity();
            o.lambda = S.conductivity();
        }
    }
    const double dh = REL_STEP * (std::abs(h) + H_SCALE_J_KG), dp = REL_STEP * p;
    double rhp, Thp, rhm, Thm, rpp, Tpp, rpm, Tpm;
    bool a, b, c, d;
    const bool okhp = eval_table(S, h + dh, p, rhp, Thp, a) && a == tp0;
    const bool okhm = eval_table(S, h - dh, p, rhm, Thm, b) && b == tp0;
    const bool okpp = eval_table(S, h, p + dp, rpp, Tpp, c) && c == tp0;
    const bool okpm = eval_table(S, h, p - dp, rpm, Tpm, d) && d == tp0;
    const double rho_h = fd(rho0, rhp, rhm, dh, okhp, okhm);
    const double rho_p = fd(rho0, rpp, rpm, dp, okpp, okpm);
    o.ddhp = rho_h;
    o.ddph = rho_p;
    if (!tp0) {
        const double T_h = fd(T0, Thp, Thm, dh, okhp, okhm);
        const double T_p = fd(T0, Tpp, Tpm, dp, okpp, okpm);
        o.cp = 1 / T_h;
        o.beta = -rho_h / (rho0 * T_h);
        o.kappa = (rho_p - rho_h * T_p / T_h) / rho0;
        o.cv = o.cp - T0 * o.beta * o.beta / (rho0 * o.kappa);
    }
}

static void variant_A(AbstractState& S, double p, double h, Out& o, bool transport) {
    S.update(CoolProp::HmassP_INPUTS, h, p);
    o.T = S.T();
    o.rho = S.rhomass();
    const bool tp = S.phase() == CoolProp::iphase_twophase;
    const double x = tp ? S.Q() : -1;
    props_A(S, h, p, o, transport);
    if (tp) {
        S.update(CoolProp::PQ_INPUTS, p, 0);
        const double hl = S.hmass();
        S.update(CoolProp::PQ_INPUTS, p, 1);
        const double hv = S.hmass();
        Out ol{}, ov{};
        props_A(S, hl - REL_STEP * (std::abs(hl) + H_SCALE_J_KG), p, ol, transport);
        props_A(S, hv + REL_STEP * (std::abs(hv) + H_SCALE_J_KG), p, ov, transport);
        o.cp = ol.cp + x * (ov.cp - ol.cp);
        o.cv = ol.cv + x * (ov.cv - ol.cv);
    }
}

// ------------------------------------------------------------ variants B, C
struct Ctx
{
    State S, E, L, V;  // table, evaluation, liquid and vapour HEOS states
    double T_c, p_c, rho_c;
};

static CoolProp::phases label(const Ctx& c, double T, double rho, double p) {
    if (T >= c.T_c && p >= c.p_c) return CoolProp::iphase_supercritical;
    if (T >= c.T_c) return CoolProp::iphase_supercritical_gas;
    if (p >= c.p_c) return CoolProp::iphase_supercritical_liquid;
    return rho > c.rho_c ? CoolProp::iphase_liquid : CoolProp::iphase_gas;
}

static void at_Trho(AbstractState& H, double T, double rho, CoolProp::phases ph) {
    H.specify_phase(ph);
    H.update(CoolProp::DmassT_INPUTS, rho, T);
}

static void variant_BC(Ctx& c, double p, double h, Out& o, bool transport, int newton, bool table_transport = false) {
    AbstractState& S = *c.S;
    S.update(CoolProp::HmassP_INPUTS, h, p);
    if (S.phase() != CoolProp::iphase_twophase) {
        if (table_transport) {
            o.eta = S.viscosity();
            o.lambda = S.conductivity();
            transport = false;
        }
        double T = S.T(), rho = S.rhomass();
        const CoolProp::phases ph = label(c, T, rho, p);
        AbstractState& E = *c.E;
        at_Trho(E, T, rho, ph);
        // newton = 0: variant B, 1: variant C (one step), n > 1: iterate
        // until kappa |p - p_in| < 1e-9 and |h - h_in| < 1e-9 (|h| + 1e5),
        // at most n steps
        for (int step = 0; step < newton; step++) {
            const double rp = E.p() - p, rh = E.hmass() - h;
            if (newton > 1 && std::abs(rp) * E.isothermal_compressibility() < 1e-9
                && std::abs(rh) < 1e-9 * (std::abs(h) + H_SCALE_J_KG))
                break;
            const double pT = E.first_partial_deriv(CoolProp::iP, CoolProp::iT, CoolProp::iDmass);
            const double pr = E.first_partial_deriv(CoolProp::iP, CoolProp::iDmass, CoolProp::iT);
            const double hT = E.first_partial_deriv(CoolProp::iHmass, CoolProp::iT, CoolProp::iDmass);
            const double hr = E.first_partial_deriv(CoolProp::iHmass, CoolProp::iDmass, CoolProp::iT);
            const double det = pT * hr - pr * hT;
            T -= (rp * hr - rh * pr) / det;
            rho -= (pT * rh - hT * rp) / det;
            at_Trho(E, T, rho, ph);
        }
        single_phase(E, o, transport);
        return;
    }
    // Two-phase: saturated densities from the table, HEOS on both sides
    S.update(CoolProp::PQ_INPUTS, p, 0);
    const double T = S.T(), rl = S.rhomass();
    S.update(CoolProp::PQ_INPUTS, p, 1);
    const double rv = S.rhomass();
    AbstractState &L = *c.L, &V = *c.V;
    at_Trho(L, T, rl, CoolProp::iphase_liquid);
    at_Trho(V, T, rv, CoolProp::iphase_gas);
    const double hl = L.hmass(), hv = V.hmass(), vl = 1 / rl, vv = 1 / rv;
    const double dTp = T * (vv - vl) / (hv - hl);
    auto side = [&](AbstractState& H, double& drho, double& dh) {
        drho = H.first_partial_deriv(CoolProp::iDmass, CoolProp::iP, CoolProp::iT)
               + H.first_partial_deriv(CoolProp::iDmass, CoolProp::iT, CoolProp::iP) * dTp;
        dh = H.first_partial_deriv(CoolProp::iHmass, CoolProp::iP, CoolProp::iT)
             + H.first_partial_deriv(CoolProp::iHmass, CoolProp::iT, CoolProp::iP) * dTp;
    };
    double drl, dhl, drv, dhv;
    side(L, drl, dhl);
    side(V, drv, dhv);
    const double x = (h - hl) / (hv - hl), v = vl + x * (vv - vl), rho = 1 / v;
    const double dxdp = -((1 - x) * dhl + x * dhv) / (hv - hl);
    const double dvdp = -(1 - x) * drl / (rl * rl) - x * drv / (rv * rv) + (vv - vl) * dxdp;
    o.T = T;
    o.rho = rho;
    o.ddhp = -rho * rho * (vv - vl) / (hv - hl);
    o.ddph = -rho * rho * dvdp;
    o.cp = L.cpmass() + x * (V.cpmass() - L.cpmass());
    o.cv = L.cvmass() + x * (V.cvmass() - L.cvmass());
}

// p-T inputs: plain HEOS against the table guess plus a converged HEOS solve
static void pT_heos(AbstractState& H, double p, double T, Out& o, bool transport) {
    H.update(CoolProp::PT_INPUTS, p, T);
    single_phase(H, o, transport);
}

static void pT_C(Ctx& c, double p, double T, Out& o, bool transport) {
    AbstractState& S = *c.S;
    S.update(CoolProp::PT_INPUTS, p, T);
    double rho = S.rhomass();
    CoolProp::phases ph;
    if (p < c.p_c && T < c.T_c) {
        S.update(CoolProp::PQ_INPUTS, p, 0);
        const double Ts = S.T(), rl = S.rhomass();
        S.update(CoolProp::PQ_INPUTS, p, 1);
        const double rv = S.rhomass();
        if (T < Ts) {
            ph = CoolProp::iphase_liquid;
            rho = std::max(rho, rl);
        } else {
            ph = CoolProp::iphase_gas;
            rho = std::min(rho, rv);
        }
    } else {
        ph = label(c, T, rho, p);
    }
    CoolProp::GuessesStructure g;
    g.T = T;
    g.rhomolar = rho / c.E->molar_mass();
    c.E->specify_phase(ph);
    c.E->update_with_guesses(CoolProp::PT_INPUTS, p, T, g);
    single_phase(*c.E, o, transport);
}

// p-T inputs: phase and guess as pT_C, then a hand-written 1-D Newton
// iteration rho_new = rho - (p(T, rho) - p_in)/(dp/drho)_T
static void pT_N(Ctx& c, double p, double T, Out& o, bool transport) {
    AbstractState& S = *c.S;
    S.update(CoolProp::PT_INPUTS, p, T);
    double rho = S.rhomass();
    CoolProp::phases ph;
    if (p < c.p_c && T < c.T_c) {
        S.update(CoolProp::PQ_INPUTS, p, 0);
        const double Ts = S.T(), rl = S.rhomass();
        S.update(CoolProp::PQ_INPUTS, p, 1);
        const double rv = S.rhomass();
        if (T < Ts) {
            ph = CoolProp::iphase_liquid;
            rho = std::max(rho, rl);
        } else {
            ph = CoolProp::iphase_gas;
            rho = std::min(rho, rv);
        }
    } else {
        ph = label(c, T, rho, p);
    }
    AbstractState& E = *c.E;
    for (int it = 0; it < 30; it++) {
        at_Trho(E, T, rho, ph);
        const double r = E.p() - p;
        if (std::abs(r) < 1e-10 * p) break;
        rho -= r / E.first_partial_deriv(CoolProp::iP, CoolProp::iDmass, CoolProp::iT);
    }
    single_phase(E, o, transport);
}

// --------------------------------------------------------------- saturation
static void sat_heos(AbstractState& H, double p, double* r) {
    H.specify_phase(CoolProp::iphase_liquid);
    H.update(CoolProp::PQ_INPUTS, p, 0);
    r[0] = H.T();
    r[1] = H.first_saturation_deriv(CoolProp::iT, CoolProp::iP);
    r[2] = H.first_saturation_deriv(CoolProp::iDmass, CoolProp::iP);
    r[3] = H.first_saturation_deriv(CoolProp::iHmass, CoolProp::iP);
    H.specify_phase(CoolProp::iphase_gas);
    H.update(CoolProp::PQ_INPUTS, p, 1);
    r[4] = H.first_saturation_deriv(CoolProp::iDmass, CoolProp::iP);
    r[5] = H.first_saturation_deriv(CoolProp::iHmass, CoolProp::iP);
    H.unspecify_phase();
}

static void sat_B(Ctx& c, double p, double* r) {
    AbstractState& S = *c.S;
    S.update(CoolProp::PQ_INPUTS, p, 0);
    const double T = S.T(), rl = S.rhomass();
    S.update(CoolProp::PQ_INPUTS, p, 1);
    const double rv = S.rhomass();
    at_Trho(*c.L, T, rl, CoolProp::iphase_liquid);
    at_Trho(*c.V, T, rv, CoolProp::iphase_gas);
    const double dTp = T * (1 / rv - 1 / rl) / (c.V->hmass() - c.L->hmass());
    r[0] = T;
    r[1] = dTp;
    r[2] = c.L->first_partial_deriv(CoolProp::iDmass, CoolProp::iP, CoolProp::iT)
           + c.L->first_partial_deriv(CoolProp::iDmass, CoolProp::iT, CoolProp::iP) * dTp;
    r[3] = c.L->first_partial_deriv(CoolProp::iHmass, CoolProp::iP, CoolProp::iT)
           + c.L->first_partial_deriv(CoolProp::iHmass, CoolProp::iT, CoolProp::iP) * dTp;
    r[4] = c.V->first_partial_deriv(CoolProp::iDmass, CoolProp::iP, CoolProp::iT)
           + c.V->first_partial_deriv(CoolProp::iDmass, CoolProp::iT, CoolProp::iP) * dTp;
    r[5] = c.V->first_partial_deriv(CoolProp::iHmass, CoolProp::iP, CoolProp::iT)
           + c.V->first_partial_deriv(CoolProp::iHmass, CoolProp::iT, CoolProp::iP) * dTp;
}

// ------------------------------------------------------------------- driver
struct Pt
{
    double p, h;
};

// Number of calls that threw in the last time_us run, per point
static int g_fails = 0;

template <class F>
static double time_us(const std::vector<Pt>& pts, F f) {
    double best = 1e30;
    g_fails = 0;
    for (const Pt& q : pts) {
        try {
            f(q.p, q.h);
        } catch (...) {
            g_fails++;
        }
    }
    for (int rep = 0; rep < 5; rep++) {
        const auto t0 = std::chrono::steady_clock::now();
        const int n_loop = 20;
        for (int k = 0; k < n_loop; k++)
            for (const Pt& q : pts) {
                try {
                    f(q.p, q.h);
                } catch (...) {
                }
            }
        const auto t1 = std::chrono::steady_clock::now();
        const double us = std::chrono::duration<double, std::micro>(t1 - t0).count() / (n_loop * pts.size());
        if (us < best) best = us;
    }
    return best;
}

// Format a time, with the number of failed points if any
static std::string cell(double us) {
    char buf[64];
    if (g_fails)
        std::snprintf(buf, sizeof(buf), "%.2f (%d failed)", us, g_fails);
    else
        std::snprintf(buf, sizeof(buf), "%.2f", us);
    return buf;
}

int main(int argc, char** argv) {
    std::vector<std::string> fluids = {"Water", "CO2", "R134a"};
    if (argc > 1) fluids.assign(argv + 1, argv + argc);
    std::printf("| fluid | transport | region | HEOS | TTSE | BICUBIC | A | B | C | C converged | C, table transport |\n");
    std::printf("|---|---|---|---|---|---|---|---|---|---|---|\n");
    std::vector<std::string> sat_lines, pT_lines;
    for (const std::string& fluid_s : fluids) {
        const char* fluid = fluid_s.c_str();
        State H = make("HEOS", fluid);
        State TT = make("TTSE&HEOS", fluid);
        State BI = make("BICUBIC&HEOS", fluid);
        Ctx c{make("SVDSBTL&HEOS", fluid), make("HEOS", fluid), make("HEOS", fluid), make("HEOS", fluid),
              H->T_critical(), H->p_critical(), H->rhomass_critical()};
        // Test points: the same grid as the Python prototype
        std::vector<Pt> single, near, far, twoph;
        std::vector<double> p_sat;
        const double pc = H->p_critical(), Tc = H->T_critical();
        const double p_min = 1.5 * H->keyed_output(CoolProp::iP_triple), T_min = H->Ttriple() + 2;
        for (double pr : {0.01, 0.05, 0.2, 0.5, 0.8, 0.95, 0.99}) {
            const double p = pr * pc;
            if (p < p_min) continue;
            p_sat.push_back(p);
            H->unspecify_phase();
            H->update(CoolProp::PQ_INPUTS, p, 0);
            const double Ts = H->T();
            for (double dT : {0.001, 0.01, 0.1, 1.0, 10.0}) {
                std::vector<Pt>& dst = dT <= 0.1 ? near : far;
                if (Ts - dT > T_min) {
                    H->specify_phase(CoolProp::iphase_liquid);
                    H->update(CoolProp::PT_INPUTS, p, Ts - dT);
                    single.push_back({p, H->hmass()});
                    dst.push_back(single.back());
                }
                H->specify_phase(CoolProp::iphase_gas);
                H->update(CoolProp::PT_INPUTS, p, Ts + dT);
                single.push_back({p, H->hmass()});
                dst.push_back(single.back());
            }
            H->unspecify_phase();
            for (double x : {0.001, 0.01, 0.5, 0.99, 0.999}) {
                H->update(CoolProp::PQ_INPUTS, p, x);
                twoph.push_back({p, H->hmass()});
            }
        }
        H->unspecify_phase();
        for (double pr : {1.2, 2.0, 4.0})
            for (double Tr : {0.8, 0.95, 1.05, 1.2, 2.0}) {
                if (Tr * Tc > H->Tmax()) continue;
                H->update(CoolProp::PT_INPUTS, pr * pc, Tr * Tc);
                single.push_back({pr * pc, H->hmass()});
                far.push_back(single.back());
            }
        Out o{};
        for (bool tr : {false, true}) {
            const char* region_names[] = {"single-phase, within 0.1 K of saturation", "single-phase, other", "two-phase"};
            for (int region = 0; region < 3; region++) {
                const std::vector<Pt>& pts = region == 0 ? near : (region == 1 ? far : twoph);
                std::string t[8];
                t[0] = cell(time_us(pts, [&](double p, double h) { plain_ph(*H, p, h, o, tr); }));
                t[1] = cell(time_us(pts, [&](double p, double h) { plain_ph(*TT, p, h, o, tr); }));
                t[2] = cell(time_us(pts, [&](double p, double h) { plain_ph(*BI, p, h, o, tr); }));
                t[3] = cell(time_us(pts, [&](double p, double h) { variant_A(*c.S, p, h, o, tr); }));
                t[4] = cell(time_us(pts, [&](double p, double h) { variant_BC(c, p, h, o, tr, 0); }));
                t[5] = cell(time_us(pts, [&](double p, double h) { variant_BC(c, p, h, o, tr, 1); }));
                t[6] = cell(time_us(pts, [&](double p, double h) { variant_BC(c, p, h, o, tr, 5); }));
                t[7] = tr ? cell(time_us(pts, [&](double p, double h) { variant_BC(c, p, h, o, tr, 1, true); })) : "";
                std::printf("| %s | %s | %s | %s | %s | %s | %s | %s | %s | %s | %s |\n", fluid, tr ? "yes" : "no",
                            region_names[region], t[0].c_str(), t[1].c_str(), t[2].c_str(), t[3].c_str(),
                            t[4].c_str(), t[5].c_str(), t[6].c_str(), t[7].c_str());
                std::fflush(stdout);
            }
        }
        // p-T inputs on the single-phase points
        std::vector<Pt> pT;
        for (const Pt& q : single) {
            H->update(CoolProp::HmassP_INPUTS, q.h, q.p);
            pT.push_back({q.p, H->T()});
        }
        H->unspecify_phase();
        for (bool tr : {false, true}) {
            const std::string t1 = cell(time_us(pT, [&](double p, double T) { pT_heos(*H, p, T, o, tr); }));
            const std::string t2 = cell(time_us(pT, [&](double p, double T) { pT_C(c, p, T, o, tr); }));
            const std::string t3 = cell(time_us(pT, [&](double p, double T) { pT_N(c, p, T, o, tr); }));
            pT_lines.push_back("| " + fluid_s + " | " + (tr ? "yes" : "no") + " | " + t1 + " | " + t2 + " | " + t3 + " |");
        }
        // Saturation: setSat_p with plain HEOS against variant B
        std::vector<Pt> sp;
        for (double p : p_sat)
            sp.push_back({p, 0});
        double r[6];
        const std::string tsH = cell(time_us(sp, [&](double p, double) { sat_heos(*H, p, r); }));
        const std::string tsB = cell(time_us(sp, [&](double p, double) { sat_B(c, p, r); }));
        sat_lines.push_back("| " + fluid_s + " | " + tsH + " | " + tsB + " |");
    }
    std::printf("\np-T inputs, single-phase, microseconds per call\n\n| fluid | transport | HEOS | C, update_with_guesses | C, own Newton |\n|---|---|---|---|---|\n");
    for (const auto& l : pT_lines)
        std::printf("%s\n", l.c_str());
    std::printf("\nsetSat_p, microseconds per call\n\n| fluid | HEOS | B |\n|---|---|---|\n");
    for (const auto& l : sat_lines)
        std::printf("%s\n", l.c_str());
    return 0;
}
