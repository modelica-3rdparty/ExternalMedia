# SVDSBTL as a starting guess for HEOS: prototype results

This note summarises the measurements made with `svdsbtl_prototype.py`
(accuracy) and `svdsbtl_benchmark.cpp` (speed) before the C++
implementation of the SVDSBTL option in `coolpropsolver.cpp`. The full
output of both programs is in `svdsbtl_prototype_output.md`.

## Variants

- **A**: SVDSBTL plus finite differences of the tables, as in commit
  5966387. cp, cv, kappa, beta, ddph, ddhp and the saturation derivatives
  come from 4 extra table look-ups per state.
- **B**: the SVDSBTL table only inverts the inputs to (T, rho). HEOS is
  then evaluated directly at (T, rho) with the phase imposed, so CoolProp
  does no iteration and no phase check. All outputs and derivatives are
  analytical HEOS values.
- **C**: like B, plus one Newton step on HEOS in (T, rho) so that
  p(T, rho) and h(T, rho) match the inputs. For p-T inputs, C is a
  converged 1-D Newton iteration in rho (see below).

Two-phase states and saturation properties (B and C): T_sat and the
saturated densities come from the table (SVDSBTL takes them from the
CoolProp SuperAncillary functions, which match HEOS to round-off),
HEOS is evaluated on the liquid and vapour side, the quality follows
from h, and the saturation derivatives from the Clausius-Clapeyron
equation. C adds one Newton step on the phase equilibrium.

## Test conditions

- CoolProp 8.0.0, `SVDSBTL&HEOS` with default options, `HEOS` as reference.
- Fluids: Water, CO2, R134a.
- p-h test points at p/p_c = 0.01, 0.05, 0.2, 0.5, 0.8, 0.95, 0.99 (above
  1.5 times the triple point pressure): subcooled liquid and superheated
  vapour at dT = 0.001, 0.01, 0.1, 1 and 10 K from the saturation line,
  two-phase at x = 0.001, 0.01, 0.5, 0.99, 0.999, and supercritical states
  at p/p_c = 1.2, 2, 4 and T/T_c = 0.8, 0.95, 1.05, 1.2, 2.
  90 to 120 points per fluid.
- In addition 2000 random single-phase points per fluid for variant C:
  dT from 1e-4 K to 30 K from the saturation line at random pressures, and
  the region around the critical point (p/p_c 0.9 to 1.3, T/T_c 0.97 to 1.1).
- Timing: C++, g++ 13 -O2, static CoolProp 8.0.0 Release build, one core of
  an Intel Xeon at 2.1 GHz, best of 5 runs over the same points. The time
  includes everything `postStateChange` reads: state, cp, cv, speed of
  sound, kappa, beta, ddph, ddhp, and viscosity and conductivity if
  "transport" is yes.

## Accuracy, p-h inputs

Worst relative error against plain HEOS over the three fluids. "State" is
rho and T, "derivatives" is the worst of cp, cv, speed of sound, kappa,
beta, ddph and ddhp. In the two-phase region the derivatives are ddph and
ddhp plus the extend_twophase values of cp, cv and speed of sound.

| region | state A | state B | state C | derivatives A | derivatives B | derivatives C | TTSE | BICUBIC |
|---|---|---|---|---|---|---|---|---|
| liquid, dT_sc <= 0.01 K | 1e-04 | 1e-04 | 7e-08 | 5e+00 | 4e-03 | 7e-06 | 6e-01 | 3e-01 |
| liquid, dT_sc = 0.1 K | 2e-06 | 2e-06 | 3e-08 | 4e-01 | 3e-05 | 5e-07 | 9e-02 | 3e-02 |
| liquid, dT_sc >= 1 K | 6e-06 | 6e-06 | 1e-08 | 3e-01 | 2e-05 | 2e-07 | 1e-01 | 4e-02 |
| vapour, dT_sh <= 0.01 K | 8e-04 | 8e-04 | 1e-06 | 6e+00 | 4e-02 | 4e-05 | 6e-01 | 3e-01 |
| vapour, dT_sh = 0.1 K | 7e-04 | 7e-04 | 4e-07 | 4e-01 | 3e-03 | 1e-06 | 2e-01 | 2e-01 |
| vapour, dT_sh >= 1 K | 9e-05 | 9e-05 | 5e-09 | 7e-02 | 2e-04 | 1e-08 | 5e-02 | 5e-02 |
| two-phase, 0.001 <= x <= 0.999 | 5e-13 | 2e-13 | 1e-11 | 7e+00 | 4e-13 | 3e-11 | 7e-01 | 7e-01 |
| supercritical | 3e-07 | 3e-07 | 3e-09 | 2e-04 | 4e-06 | 1e-08 | 1e-02 | 5e-04 |

The TTSE and BICUBIC columns are derivatives only. In the same test, TTSE
threw an error for 18 to 30 of the 90 to 120 points per fluid (mostly
transport properties and two-phase states), BICUBIC for 3 water points.

Consistency with the inputs. Variant B returns a HEOS state at the table
(T, rho), which differs from the input pair by the table error: up to
160 J/kg in h for vapour within 0.1 K of the dew line (CO2 and water) and
up to 0.8 % in p for liquid R134a (the liquid is stiff, so a small density
error gives a large pressure error). After the Newton step of C the
mismatch is below 0.2 J/kg in h and 6e-5 in p/p for all points, including
the random points.

Random points (6000 points, no failures):

| Newton | Water | CO2 | R134a |
|---|---|---|---|
| one step (C) | 8e-7 | 3e-5 (cp, p/p_c = 0.98, 0.002 K above the dew line) | 2e-6 |
| to convergence, at most 5 steps | 3e-7 | 3e-5 (beta, p/p_c = 0.997, 0.1 K below T_c) | 1e-7 |

The "converged" rows use the criterion of the proposed design below.
Their maximum p mismatch (1e-4 for water, 6e-5 for R134a) occurs for
low-pressure liquid where the table point already meets the criterion:
there rho is right to 1e-9, but p of a stiff liquid is so sensitive to
rho that 1e-9 in rho is 1e-4 in p at 1 kPa. Plain HEOS returns the input
p and h in this situation, and so should the C++ code.

With one step, the worst CO2 point keeps a mismatch of 0.19 J/kg in h,
and so close to the critical point cp changes by 3e-5 over that
distance. Iterating to convergence removes that; the remaining 3e-5 is
right at the critical point, where beta and cp diverge. (The plain HEOS
reference is self-consistent to 3e-7 at these points: a p-T flash and a
p-h flash at the same state agree to that level.)

Phase classification: on none of the test points did the SVDSBTL table
classify a point as one-phase when HEOS saw two-phase, or the other way
around. The old version stopped the simulation in that case, see
postStateChange_SVDSBTL in 5966387.

Saturation properties (setSat_p: T_sat, rho_l, rho_v, h_l, h_v, dTp,
ddldp, dhldp, ddvdp, dhvdp), worst over the fluids: A 2e-7, B 2e-14,
C 2e-11. A Newton step on the phase equilibrium does not improve B,
because the SuperAncillary values are already exact to round-off.

## Accuracy, p-T and p-s inputs

- **p-s**: the same picture as p-h. B: up to 8e-3 in the derivatives
  within 0.1 K of saturation. C (one Newton step on p and s): 4e-5 or better.
- **p-T**: the SVDSBTL p-T table is a poor starting guess near the
  saturation line. Within about 0.01 K it can return the density of the
  wrong phase (water at p/p_c = 0.5: 671 kg/m3 for a vapour state of
  63 kg/m3), and near the critical point (p/p_c = 0.95 to 0.99) the
  density is off by 10 to 30 %. One Newton step cannot repair that.
  Working method: decide the phase by comparing T with T_sat(p) from the
  SuperAncillary, move the table density to the correct side of the
  saturated density if needed, and iterate rho to convergence (1-D Newton,
  |p - p_in| < 1e-10 p). Result: rho within 3e-9 of plain HEOS, the
  derivatives within 3e-5 (CO2 at p/p_c = 0.99, within 0.01 K of the
  dew line) and within 7e-6 for water and R134a, no failures.

## Speed

Microseconds per p-h call, all outputs that postStateChange reads.
"C conv." iterates the Newton step until the inputs match (see the
proposed design below).

| fluid | transport | region | HEOS | TTSE | BICUBIC | A | B | C | C conv. |
|---|---|---|---|---|---|---|---|---|---|
| Water | no | single-phase, within 0.1 K of sat. | 140 | 0.7 | 7.6 | 4.4 | 4.2 | 7.7 | 9.4 |
| Water | no | single-phase, other | 206 | 0.7 | 7.2 | 3.2 | 4.1 | 8.1 | 6.2 |
| Water | no | two-phase | 7.2 | 0.8 | 1.2 | 10.7 | 7.8 | 7.7 | 7.7 |
| Water | yes | single-phase, within 0.1 K of sat. | 153 | 1.7 | 7.5 | 4.5 | 16.6 | 19.5 | 21.4 |
| Water | yes | single-phase, other | 214 | 1.1 | 7.2 | 3.5 | 15.9 | 19.6 | 18.4 |
| CO2 | no | single-phase, within 0.1 K of sat. | 144 | 0.7 | 7.4 | 4.2 | 4.3 | 7.8 | 9.2 |
| CO2 | no | single-phase, other | 203 | 0.7 | 7.2 | 3.0 | 4.1 | 7.6 | 7.0 |
| CO2 | no | two-phase | 7.4 | 0.6 | 0.6 | 10.5 | 7.9 | 7.9 | 7.9 |
| CO2 | yes | single-phase, within 0.1 K of sat. | 149 | 1.7 | 7.4 | 4.5 | 10.8 | 14.3 | 15.7 |
| CO2 | yes | single-phase, other | 208 | 1.1 | 7.2 | 3.3 | 10.6 | 14.1 | 13.4 |
| R134a | no | single-phase, within 0.1 K of sat. | 33 | 0.7 | 7.5 | 4.3 | 2.1 | 3.3 | 3.6 |
| R134a | no | single-phase, other | 44 | 0.7 | 7.1 | 3.0 | 1.9 | 3.1 | 2.8 |
| R134a | no | two-phase | 2.9 | 0.6 | 0.6 | 10.5 | 3.5 | 3.5 | 3.5 |
| R134a | yes | single-phase, within 0.1 K of sat. | 35 | 2.0 | 7.5 | 4.6 | 4.0 | 5.3 | 5.5 |
| R134a | yes | single-phase, other | 47 | 1.1 | 7.3 | 3.4 | 3.8 | 5.1 | 4.8 |

The two-phase timings with transport are the same as without (the
transport properties are not evaluated in the two-phase region). The TTSE
numbers are flattered by the calls that fail quickly, see above.

Other inputs and the saturation routine, microseconds per call:

| fluid | p-T HEOS | p-T C (converged Newton) | setSat_p HEOS | setSat_p B |
|---|---|---|---|---|
| Water | 25 | 10 | 18.8 | 7.2 |
| CO2 | 23 | 11 | 19.4 | 7.3 |
| R134a | 6.0 | 3.2 | 5.9 | 3.1 |

(p-T without transport. HEOS `update_with_guesses` with PT_INPUTS also
works from the same guess, but it saves only 15 % against plain HEOS, so
a hand-written Newton loop is used.)

Notes on the numbers:

- Plain HEOS p-h is slow in CoolProp 8.0.0: 120 to 300 us per call for
  water on this machine, also far from the saturation line. p-T is
  15 to 30 us. This is what makes the table worth using.
- The Newton step of C costs about 3.5 us for water and CO2 and 1.3 us for
  R134a (one extra HEOS evaluation at (T, rho) plus four derivatives).
- HEOS transport properties cost 12 us per call for water, 6 us for CO2
  and 2 us for R134a. The SVDSBTL table also stores viscosity and
  conductivity, which would save that time, but near the saturation line
  the tabulated conductivity is off by up to 8 % (CO2), 0.9 % (water)
  and 0.4 % (R134a), and the viscosity by up to 0.15 %.
- `CoolProp::HEOS` p-h in the two-phase region is fast (SuperAncillary),
  so the table brings no speed advantage there.

## Table build

First use of a fluid builds the p-h and p-T tables (about 100 s for water
and CO2, 30 s for R134a with 4 sampling threads on this machine, several
times longer with the default single thread). The p-s and d-T tables are
built on first use of those inputs (about 90 s and 10 s for water). The
tables are stored in `~/.CoolProp/SVDTables` (14 to 16 MB per table) and
loaded in well under a second afterwards.

## Side finding: TTSE crash in CoolProp 8.0.0

In the C++ benchmark, `TTSE&HEOS` crashed with a segmentation fault (in
`TTSEBackend::find_native_nearest_good_indices`) for CO2 and R134a, but
only in the process that built the TTSE tables for the first time. With
the tables in the cache, the same run completed. This affects the
existing enable_TTSE option, not SVDSBTL.

## Proposed C++ design

Based on these numbers, variant C for single-phase states, the HEOS
two-phase code that exists today for two-phase states, and B for the
saturation routines:

1. Keep `state` as the reference backend (HEOS, or REFPROP/IF97 if given)
   and add a second state `table_state` with the SVDSBTL backend. Because
   `state` ends up holding the HEOS state at the exact input,
   `postStateChange` stays unchanged and all outputs, analytical
   derivatives and transport properties come from HEOS.
2. setState_ph, single-phase: table to (T, rho); phase label from T, p and
   rho (SVDSBTL reports `iphase_not_imposed` for single-phase points, so
   the label has to be derived); HEOS at (T, rho) with that phase imposed;
   Newton steps on (p, h) until kappa |p - p_in| < 1e-9 and
   |h - h_in| < 1e-9 (|h| + 1e5 J/kg), at most 5 steps. The pressure
   residual is measured by the density change it implies (kappa times
   the residual), because p computed from (T, rho) of a stiff liquid
   carries a round-off error larger than 1e-9 p at low pressure (water
   at 1 to 10 kPa); a plain 1e-9 p tolerance is never met there. On 4500
   random points this criterion needed 0 steps for 18 to 26 % of the
   points, 1 step for 50 to 80 % and never more than 2, so the cost is
   close to that of C. The returned p and h are the inputs, as for the
   plain HEOS flash. If the table fails, a Newton step gives a non-finite
   value or the iteration does not converge, fall back to the plain HEOS
   p-h flash. No error is raised for a classification difference.
3. setState_ph, two-phase (table says two-phase): plain HEOS p-h flash,
   as today. It is as fast as the table route, exact, and keeps
   twophase_derivsmoothing_xend, rho_smoothing_xend and extend_twophase
   working unchanged, so these options no longer need to be rejected.
4. setState_ps: as setState_ph with s in place of h.
5. setState_pT: phase from T against T_sat(p) from the table's saturation
   functions, table density as guess (moved to the right side of the
   saturated density), 1-D Newton in rho to convergence, fallback to plain
   HEOS.
6. setState_dT: plain HEOS. HEOS is explicit in (T, rho), the table brings
   nothing here.
7. setState_hs: plain HEOS h-s flash (no SVDSBTL table for h-s).
8. setSat_p and setSat_T: T_sat and the saturated densities from the
   table, HEOS evaluated on both sides, dTp from Clausius-Clapeyron, and
   ddldp, dhldp, ddvdp, dhvdp from the HEOS derivatives along the
   saturation line. Surface tension from HEOS. No Newton step.
9. Transport properties from HEOS (accurate near the saturation line).

Expected result: HEOS accuracy (within 1e-6 on all outputs, up to
3e-5 right at the critical point) at 8 to 20 us per single-phase p-h call for water instead of
150 to 230 us, and 3 to 5 us instead of 33 to 46 us for R134a.

Remaining limitations: pure fluids only; first use of a fluid builds the
tables (tens of seconds to a few minutes) and needs a writable cache
folder; the speed gain is in the single-phase region and in setSat_p,
not in the two-phase region; the result matches the input pair to
1e-9 relative, not exactly.
