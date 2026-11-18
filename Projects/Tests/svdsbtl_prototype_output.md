# SVDSBTL prototype: full output

Generated with CoolProp 8.0.0. Accuracy: `python svdsbtl_prototype.py --random`. Timing: `svdsbtl_benchmark` (g++ 13 -O2, static CoolProp 8.0.0 Release build, Intel Xeon 2.1 GHz). See svdsbtl_prototype_results.md for the summary and the test conditions.

## Accuracy

CoolProp 8.0.0


### Summary, p-h inputs, worst case over Water, CO2, R134a

Maximum relative error against plain HEOS. State: rho and T. Derivatives: cp, cv, speed of sound, kappa, beta, ddph, ddhp. Input mismatch of B: max |h(T, rho) - h_in| in J/kg (C: below 0.2 J/kg).

| region | state A | state B | state C | derivatives A | derivatives B | derivatives C | h mismatch B | derivatives TTSE | derivatives BICUBIC |
|---|---|---|---|---|---|---|---|---|---|
| liquid, dT_sc <= 0.01 K | 1e-04 | 1e-04 | 7e-08 | 5e+00 | 4e-03 | 7e-06 | 8.2 | 6e-01 | 3e-01 |
| liquid, dT_sc = 0.1 K | 2e-06 | 2e-06 | 3e-08 | 4e-01 | 3e-05 | 5e-07 | 0.56 | 9e-02 | 3e-02 |
| liquid, dT_sc >= 1 K | 6e-06 | 6e-06 | 1e-08 | 3e-01 | 2e-05 | 2e-07 | 2.7 | 1e-01 | 4e-02 |
| vapour, dT_sh <= 0.01 K | 8e-04 | 8e-04 | 1e-06 | 6e+00 | 4e-02 | 4e-05 | 1.6e+02 | 6e-01 | 3e-01 |
| vapour, dT_sh = 0.1 K | 7e-04 | 7e-04 | 4e-07 | 4e-01 | 3e-03 | 1e-06 | 1.3e+02 | 2e-01 | 2e-01 |
| vapour, dT_sh >= 1 K | 9e-05 | 9e-05 | 5e-09 | 7e-02 | 2e-04 | 1e-08 | 8.6 | 5e-02 | 5e-02 |
| two-phase, 0.001 <= x <= 0.999 | 5e-13 | 2e-13 | 1e-11 | 7e+00 | 4e-13 | 3e-11 | 0 | 7e-01 | 7e-01 |
| supercritical | 3e-07 | 3e-07 | 3e-09 | 2e-04 | 4e-06 | 1e-08 | 0.14 | 1e-02 | 5e-04 |

Saturation properties (setSat_p), worst over all props and fluids: A 2e-07 (T_sat, rho, h exact from the table, derivatives by finite differences), B 2e-14, C 2e-11.

### Details per fluid

#### Water

120 p-h test points, 0 with a different phase (one- or two-phase) in the table than in HEOS. Failed calls: A 0, B 0, C 0, TTSE 28, BICUBIC 3.

Maximum relative error against plain HEOS, p-h inputs, cells are A / B / C:

| region | rho | T | cp | cv | a | kappa | beta | ddph | ddhp |
|---|---|---|---|---|---|---|---|---|---|
| liquid dT_sc=0.001 K | 6e-05 / 6e-05 / 2e-08 | 9e-06 / 9e-06 / 2e-09 | 4e-01 / 2e-03 / 7e-06 | 5e+00 / 3e-04 / 2e-07 | 3e-04 / 4e-04 / 2e-07 | 2e+00 / 3e-03 / 7e-06 | 5e-01 / 3e-03 / 7e-06 | 1e+00 / 7e-04 / 3e-07 | 6e-02 / 3e-04 / 1e-07 |
| liquid dT_sc=0.01 K | 3e-05 / 3e-05 / 6e-09 | 7e-06 / 7e-06 / 1e-09 | 2e-01 / 9e-04 / 1e-07 | 1e+00 / 1e-04 / 2e-08 | 1e-04 / 2e-04 / 3e-08 | 2e+00 / 1e-03 / 2e-07 | 2e-01 / 1e-03 / 1e-07 | 1e+00 / 4e-04 / 5e-08 | 5e-02 / 2e-04 / 2e-08 |
| liquid dT_sc=0.1 K | 2e-06 / 2e-06 / 3e-08 | 6e-07 / 6e-07 / 7e-10 | 1e-02 / 1e-05 / 4e-07 | 1e-01 / 2e-06 / 5e-08 | 4e-06 / 8e-06 / 7e-08 | 2e-01 / 3e-05 / 5e-07 | 2e-02 / 2e-05 / 4e-07 | 1e-01 / 1e-05 / 1e-07 | 1e-02 / 9e-06 / 3e-08 |
| liquid dT_sc=1 K | 5e-08 / 5e-08 / 2e-09 | 3e-10 / 3e-10 / 7e-11 | 3e-06 / 6e-07 / 1e-08 | 1e-04 / 9e-08 / 2e-09 | 1e-06 / 1e-07 / 4e-09 | 4e-05 / 9e-07 / 2e-08 | 9e-06 / 7e-07 / 2e-08 | 1e-04 / 2e-07 / 7e-09 | 6e-06 / 7e-08 / 2e-09 |
| liquid dT_sc=10 K | 5e-08 / 5e-08 / 2e-10 | 3e-10 / 3e-10 / 2e-10 | 2e-07 / 3e-07 / 2e-09 | 8e-05 / 4e-08 / 2e-10 | 3e-07 / 1e-07 / 9e-10 | 3e-05 / 6e-07 / 3e-09 | 6e-06 / 4e-07 / 3e-09 | 7e-05 / 2e-07 / 2e-09 | 6e-06 / 1e-07 / 1e-09 |
| vapour dT_sh=0.001 K | 1e-04 / 1e-04 / 2e-08 | 1e-04 / 1e-04 / 3e-08 | 5e-01 / 2e-03 / 2e-07 | 6e+00 / 5e-04 / 1e-07 | 3e-04 / 1e-04 / 2e-08 | 5e-01 / 1e-03 / 2e-07 | 6e-01 / 2e-03 / 2e-07 | 7e-01 / 2e-04 / 3e-08 | 5e-02 / 1e-04 / 2e-08 |
| vapour dT_sh=0.01 K | 5e-05 / 5e-05 / 7e-08 | 1e-04 / 1e-04 / 3e-08 | 3e-01 / 8e-04 / 1e-06 | 1e+00 / 5e-04 / 1e-07 | 2e-04 / 1e-04 / 6e-08 | 2e-01 / 8e-04 / 1e-06 | 3e-01 / 8e-04 / 1e-06 | 8e-02 / 2e-04 / 1e-07 | 3e-02 / 1e-05 / 9e-08 |
| vapour dT_sh=0.1 K | 5e-05 / 5e-05 / 1e-08 | 1e-04 / 1e-04 / 2e-08 | 1e-01 / 4e-04 / 1e-06 | 1e-01 / 4e-04 / 7e-08 | 1e-04 / 9e-05 / 2e-08 | 4e-02 / 2e-04 / 1e-06 | 1e-01 / 4e-04 / 1e-06 | 7e-03 / 1e-04 / 3e-08 | 3e-02 / 8e-06 / 5e-08 |
| vapour dT_sh=1 K | 5e-06 / 5e-06 / 2e-09 | 9e-06 / 9e-06 / 5e-10 | 1e-02 / 1e-05 / 9e-09 | 8e-03 / 1e-05 / 4e-09 | 1e-05 / 5e-06 / 2e-09 | 2e-03 / 2e-05 / 5e-09 | 2e-02 / 2e-05 / 1e-08 | 4e-04 / 9e-06 / 3e-09 | 6e-03 / 9e-07 / 4e-09 |
| vapour dT_sh=10 K | 6e-09 / 6e-09 / 2e-11 | 7e-10 / 7e-10 / 2e-11 | 7e-07 / 1e-08 / 2e-10 | 5e-06 / 2e-09 / 2e-10 | 3e-08 / 2e-09 / 3e-11 | 1e-06 / 6e-09 / 1e-11 | 6e-07 / 1e-08 / 8e-11 | 3e-06 / 3e-09 / 2e-11 | 5e-07 / 8e-09 / 7e-11 |
| two-phase x=0.001 | 5e-13 / 2e-13 / 9e-12 | 0 / 0 / 2e-14 | 5e-01 / 3e-15 / 2e-11 | 6e+00 / 5e-15 / 2e-12 | 4e-04 / 6e-16 / 2e-12 | - / - / - | - / - / - | 7e-08 / 4e-13 / 2e-11 | 1e-06 / 4e-13 / 2e-11 |
| two-phase x=0.01 | 1e-13 / 4e-14 / 2e-12 | 0 / 0 / 2e-14 | 5e-01 / 2e-15 / 2e-11 | 6e+00 / 5e-15 / 2e-12 | 4e-04 / 5e-16 / 2e-12 | - / - / - | - / - / - | 3e-09 / 8e-14 / 7e-12 | 8e-08 / 9e-14 / 4e-12 |
| two-phase x=0.5 | 3e-15 / 5e-16 / 3e-13 | 0 / 0 / 2e-14 | 6e-01 / 5e-14 / 8e-12 | 8e-01 / 4e-15 / 1e-13 | 2e-04 / 5e-16 / 5e-13 | - / - / - | - / - / - | 1e-08 / 2e-14 / 1e-12 | 2e-09 / 2e-15 / 5e-13 |
| two-phase x=0.99 | 2e-15 / 9e-16 / 3e-13 | 0 / 0 / 2e-14 | 6e-01 / 7e-14 / 2e-11 | 7e+00 / 2e-15 / 2e-12 | 5e-04 / 5e-16 / 6e-13 | - / - / - | - / - / - | 2e-08 / 5e-14 / 8e-12 | 1e-09 / 7e-16 / 3e-13 |
| two-phase x=0.999 | 3e-15 / 7e-16 / 3e-13 | 0 / 0 / 2e-14 | 6e-01 / 8e-14 / 2e-11 | 7e+00 / 2e-15 / 2e-12 | 5e-04 / 9e-16 / 6e-13 | - / - / - | - / - / - | 2e-08 / 6e-14 / 8e-12 | 1e-09 / 1e-15 / 3e-13 |
| supercritical | 2e-07 / 2e-07 / 1e-09 | 8e-08 / 8e-08 / 5e-10 | 2e-05 / 5e-07 / 6e-09 | 9e-05 / 8e-08 / 6e-10 | 8e-06 / 4e-07 / 2e-09 | 9e-05 / 1e-06 / 9e-09 | 4e-05 / 1e-06 / 1e-08 | 2e-05 / 8e-07 / 5e-09 | 2e-05 / 4e-07 / 6e-09 |

Consistency with the inputs, max |p - p_in|/p_in and max |h - h_in| in J/kg, B / C:

| region | p rel. | h J/kg |
|---|---|---|
| liquid dT_sc=0.001 K | 2e-03 / 5e-08 | 8.2 / 0.0041 |
| liquid dT_sc=0.01 K | 2e-03 / 3e-08 | 3.2 / 0.0011 |
| liquid dT_sc=0.1 K | 1e-04 / 5e-10 | 0.56 / 4.6e-06 |
| liquid dT_sc=1 K | 8e-08 / 2e-11 | 0.028 / 4.3e-09 |
| liquid dT_sc=10 K | 1e-07 / 4e-11 | 0.027 / 3.7e-09 |
| vapour dT_sh=0.001 K | 2e-04 / 2e-08 | 1.6e+02 / 0.033 |
| vapour dT_sh=0.01 K | 2e-04 / 2e-08 | 1.5e+02 / 0.032 |
| vapour dT_sh=0.1 K | 1e-04 / 1e-08 | 1.3e+02 / 0.022 |
| vapour dT_sh=1 K | 1e-05 / 3e-11 | 7.4 / 2.8e-05 |
| vapour dT_sh=10 K | 2e-09 / 1e-15 | 0.0026 / 1.9e-09 |
| two-phase x=0.001 | 0 / 0 | 0 / 0 |
| two-phase x=0.01 | 0 / 0 | 0 / 0 |
| two-phase x=0.5 | 0 / 0 | 0 / 0 |
| two-phase x=0.99 | 0 / 0 | 0 / 0 |
| two-phase x=0.999 | 0 / 0 | 0 / 0 |
| supercritical | 1e-06 / 7e-13 | 0.14 / 2e-08 |

Saturation properties at p, maximum relative error, A / B / C:

| T_sat | rho_l | rho_v | h_l | h_v | dTp | ddldp | dhldp | ddvdp | dhvdp |
|---|---|---|---|---|---|---|---|---|---|
| 0 / 0 / 2e-14 | 0 / 0 / 7e-13 | 0 / 0 / 1e-12 | 5e-15 / 2e-15 / 2e-13 | 8e-16 / 2e-16 / 3e-13 | 5e-10 / 2e-15 / 5e-13 | 1e-07 / 1e-14 / 1e-11 | 1e-07 / 1e-14 / 1e-11 | 2e-07 / 1e-14 / 2e-11 | 1e-07 / 2e-14 / 2e-11 |

Saturation pressure consistency, max |p(T_sat, rho) - p|/p: B 2e-11, C 1e-11

Single-phase p-T and p-s inputs, maximum relative error, B / C. For p-T, C is a converged Newton iteration from the table guess. near: within 0.1 K of saturation, far: the other single-phase points. Failed calls p-T: B 0, C 0; p-s: B 0, C 0.

| inputs | region | rho | T | cp | cv | a | kappa | beta | ddph | ddhp | input mismatch |
|---|---|---|---|---|---|---|---|---|---|---|---|
| pT | near | 1e+01 / 9e-10 | 0 / 0 | 9e-01 / 7e-06 | 2e-01 / 2e-07 | 7e-01 / 2e-07 | 9e+00 / 7e-06 | 1e+00 / 7e-06 | 6e-01 / 3e-07 | 5e+00 / 1e-07 | 2e-02 / 5e-11 |
| pT | far | 7e-03 / 5e-11 | 0 / 0 | 4e-02 / 9e-09 | 5e-03 / 2e-09 | 3e-03 / 3e-09 | 3e-02 / 1e-08 | 4e-02 / 1e-08 | 6e-03 / 6e-09 | 9e-03 / 5e-09 | 3e-03 / 9e-11 |
| ps | near | 1e-04 / 8e-08 | 3e-05 / 1e-08 | 6e-03 / 5e-06 | 6e-04 / 3e-07 | 8e-04 / 5e-07 | 6e-03 / 5e-06 | 6e-03 / 5e-06 | 1e-03 / 8e-07 | 5e-04 / 3e-07 | 3e-03 / 2e-07 |
| ps | far | 4e-07 / 2e-09 | 2e-07 / 6e-10 | 2e-06 / 7e-09 | 3e-07 / 1e-09 | 1e-06 / 2e-09 | 4e-06 / 8e-09 | 3e-06 / 1e-08 | 2e-06 / 5e-09 | 2e-06 / 6e-09 | 2e-05 / 1e-10 |

#### CO2

90 p-h test points, 0 with a different phase (one- or two-phase) in the table than in HEOS. Failed calls: A 0, B 0, C 0, TTSE 18, BICUBIC 0.

Maximum relative error against plain HEOS, p-h inputs, cells are A / B / C:

| region | rho | T | cp | cv | a | kappa | beta | ddph | ddhp |
|---|---|---|---|---|---|---|---|---|---|
| liquid dT_sc=0.001 K | 2e-05 / 2e-05 / 7e-08 | 3e-06 / 3e-06 / 3e-10 | 2e-01 / 5e-04 / 5e-06 | 2e+00 / 1e-04 / 5e-07 | 1e-04 / 1e-04 / 4e-07 | 3e-01 / 7e-04 / 6e-06 | 2e-01 / 6e-04 / 5e-06 | 3e-01 / 2e-04 / 7e-07 | 4e-02 / 8e-05 / 1e-07 |
| liquid dT_sc=0.01 K | 4e-06 / 4e-06 / 1e-10 | 9e-07 / 9e-07 / 2e-11 | 3e-02 / 4e-05 / 3e-09 | 3e-01 / 8e-06 / 6e-10 | 8e-06 / 2e-05 / 5e-10 | 1e-01 / 7e-05 / 4e-09 | 4e-02 / 6e-05 / 3e-09 | 1e-01 / 3e-05 / 9e-10 | 2e-02 / 2e-05 / 2e-10 |
| liquid dT_sc=0.1 K | 1e-07 / 1e-07 / 3e-11 | 3e-11 / 3e-11 / 1e-11 | 2e-06 / 2e-06 / 1e-09 | 3e-04 / 4e-07 / 3e-10 | 3e-06 / 4e-07 / 9e-10 | 2e-05 / 2e-06 / 3e-09 | 2e-05 / 2e-06 / 2e-09 | 6e-05 / 6e-07 / 1e-09 | 2e-05 / 1e-07 / 1e-09 |
| liquid dT_sc=1 K | 2e-07 / 2e-07 / 1e-08 | 7e-10 / 7e-10 / 9e-10 | 5e-06 / 2e-06 / 1e-07 | 2e-04 / 3e-07 / 1e-08 | 2e-06 / 5e-07 / 3e-08 | 3e-05 / 3e-06 / 2e-07 | 3e-06 / 2e-06 / 1e-07 | 2e-04 / 8e-07 / 6e-08 | 8e-06 / 3e-07 / 2e-08 |
| liquid dT_sc=10 K | 4e-08 / 4e-08 / 1e-09 | 8e-10 / 8e-10 / 4e-10 | 1e-07 / 2e-07 / 4e-09 | 2e-04 / 3e-08 / 7e-10 | 1e-07 / 1e-07 / 4e-09 | 8e-05 / 4e-07 / 1e-08 | 5e-07 / 3e-07 / 8e-09 | 1e-04 / 2e-07 / 6e-09 | 5e-07 / 1e-07 / 3e-09 |
| vapour dT_sh=0.001 K | 8e-04 / 8e-04 / 1e-06 | 6e-04 / 6e-04 / 1e-07 | 8e-01 / 4e-02 / 4e-05 | 5e+00 / 5e-03 / 4e-06 | 8e-03 / 2e-03 / 2e-06 | 7e-01 / 4e-02 / 3e-05 | 8e-01 / 4e-02 / 4e-05 | 8e-02 / 2e-03 / 3e-06 | 2e-01 / 4e-04 / 2e-06 |
| vapour dT_sh=0.01 K | 8e-04 / 8e-04 / 1e-06 | 5e-04 / 5e-04 / 7e-08 | 6e-01 / 2e-02 / 4e-05 | 4e+00 / 3e-03 / 2e-06 | 2e-03 / 9e-04 / 7e-07 | 5e-01 / 1e-02 / 4e-05 | 6e-01 / 2e-02 / 4e-05 | 8e-02 / 1e-03 / 1e-06 | 2e-01 / 3e-04 / 2e-06 |
| vapour dT_sh=0.1 K | 7e-04 / 7e-04 / 4e-07 | 4e-04 / 4e-04 / 5e-08 | 4e-01 / 3e-03 / 1e-06 | 3e-01 / 9e-04 / 3e-07 | 3e-04 / 4e-04 / 1e-07 | 2e-01 / 3e-03 / 8e-07 | 4e-01 / 3e-03 / 1e-06 | 5e-02 / 7e-04 / 2e-07 | 1e-01 / 2e-04 / 6e-07 |
| vapour dT_sh=1 K | 9e-05 / 9e-05 / 5e-09 | 5e-05 / 5e-05 / 1e-09 | 3e-02 / 1e-05 / 1e-09 | 2e-02 / 3e-06 / 7e-10 | 2e-05 / 2e-05 / 2e-10 | 1e-02 / 2e-04 / 5e-09 | 7e-02 / 7e-05 / 5e-10 | 6e-04 / 5e-05 / 4e-10 | 4e-02 / 3e-05 / 4e-09 |
| vapour dT_sh=10 K | 1e-08 / 1e-08 / 1e-09 | 6e-09 / 6e-09 / 8e-10 | 1e-05 / 2e-08 / 4e-09 | 8e-05 / 5e-09 / 1e-09 | 7e-09 / 7e-09 / 6e-10 | 3e-05 / 4e-08 / 5e-09 | 1e-05 / 3e-08 / 6e-09 | 2e-05 / 1e-08 / 1e-09 | 3e-06 / 3e-09 / 3e-09 |
| two-phase x=0.001 | 6e-14 / 5e-16 / 1e-13 | 0 / 0 / 2e-15 | 4e-01 / 3e-16 / 2e-12 | 4e+00 / 0 / 2e-13 | 4e-04 / 1e-16 / 2e-13 | - / - / - | - / - / - | 6e-09 / 2e-15 / 2e-12 | 4e-08 / 7e-16 / 4e-13 |
| two-phase x=0.01 | 5e-14 / 1e-15 / 1e-13 | 0 / 0 / 2e-15 | 4e-01 / 2e-16 / 2e-12 | 4e+00 / 0 / 2e-13 | 5e-04 / 0 / 2e-13 | - / - / - | - / - / - | 6e-09 / 3e-15 / 2e-12 | 3e-08 / 3e-15 / 4e-13 |
| two-phase x=0.5 | 2e-15 / 4e-16 / 9e-14 | 0 / 0 / 2e-15 | 7e-01 / 1e-16 / 1e-11 | 3e-01 / 0 / 1e-12 | 5e-03 / 1e-16 / 4e-13 | - / - / - | - / - / - | 1e-08 / 8e-16 / 3e-12 | 1e-09 / 7e-16 / 2e-13 |
| two-phase x=0.99 | 3e-15 / 2e-16 / 1e-13 | 0 / 0 / 2e-15 | 8e-01 / 2e-16 / 2e-11 | 5e+00 / 0 / 3e-12 | 1e-02 / 2e-16 / 1e-12 | - / - / - | - / - / - | 2e-08 / 2e-15 / 6e-12 | 1e-09 / 7e-16 / 1e-13 |
| two-phase x=0.999 | 3e-15 / 4e-16 / 1e-13 | 0 / 0 / 2e-15 | 8e-01 / 3e-16 / 2e-11 | 5e+00 / 0 / 3e-12 | 1e-02 / 3e-16 / 1e-12 | - / - / - | - / - / - | 2e-08 / 3e-15 / 6e-12 | 1e-09 / 1e-15 / 1e-13 |
| supercritical | 3e-07 / 3e-07 / 3e-09 | 2e-07 / 2e-07 / 1e-09 | 6e-05 / 2e-06 / 3e-09 | 2e-04 / 2e-07 / 1e-10 | 1e-05 / 1e-06 / 1e-09 | 1e-04 / 4e-06 / 4e-09 | 9e-05 / 4e-06 / 6e-09 | 1e-04 / 2e-06 / 3e-09 | 3e-05 / 1e-06 / 6e-09 |

Consistency with the inputs, max |p - p_in|/p_in and max |h - h_in| in J/kg, B / C:

| region | p rel. | h J/kg |
|---|---|---|
| liquid dT_sc=0.001 K | 6e-05 / 4e-09 | 0.98 / 7.8e-05 |
| liquid dT_sc=0.01 K | 3e-05 / 5e-10 | 0.048 / 2.3e-07 |
| liquid dT_sc=0.1 K | 1e-08 / 6e-14 | 0.012 / 5.2e-10 |
| liquid dT_sc=1 K | 1e-07 / 1e-13 | 0.02 / 5.2e-10 |
| liquid dT_sc=10 K | 2e-07 / 3e-13 | 0.0048 / 5.8e-10 |
| vapour dT_sh=0.001 K | 2e-03 / 6e-07 | 1.5e+02 / 0.18 |
| vapour dT_sh=0.01 K | 2e-03 / 5e-07 | 1.4e+02 / 0.11 |
| vapour dT_sh=0.1 K | 1e-03 / 3e-07 | 1e+02 / 0.027 |
| vapour dT_sh=1 K | 1e-04 / 5e-09 | 8.6 / 0.0001 |
| vapour dT_sh=10 K | 2e-08 / 8e-16 | 0.0011 / 1.7e-10 |
| two-phase x=0.001 | 0 / 0 | 0 / 0 |
| two-phase x=0.01 | 0 / 0 | 0 / 0 |
| two-phase x=0.5 | 0 / 0 | 0 / 0 |
| two-phase x=0.99 | 0 / 0 | 0 / 0 |
| two-phase x=0.999 | 0 / 0 | 0 / 0 |
| supercritical | 5e-06 / 2e-12 | 0.096 / 1.9e-08 |

Saturation properties at p, maximum relative error, A / B / C:

| T_sat | rho_l | rho_v | h_l | h_v | dTp | ddldp | dhldp | ddvdp | dhvdp |
|---|---|---|---|---|---|---|---|---|---|
| 0 / 0 / 2e-15 | 0 / 0 / 3e-14 | 0 / 0 / 1e-12 | 5e-15 / 0 / 1e-14 | 1e-15 / 0 / 3e-13 | 7e-10 / 3e-16 / 4e-13 | 2e-07 / 1e-15 / 3e-13 | 1e-07 / 1e-15 / 2e-13 | 2e-07 / 2e-15 / 1e-11 | 2e-07 / 2e-15 / 1e-11 |

Saturation pressure consistency, max |p(T_sat, rho) - p|/p: B 4e-13, C 2e-13

Single-phase p-T and p-s inputs, maximum relative error, B / C. For p-T, C is a converged Newton iteration from the table guess. near: within 0.1 K of saturation, far: the other single-phase points. Failed calls p-T: B 0, C 0; p-s: B 0, C 0.

| inputs | region | rho | T | cp | cv | a | kappa | beta | ddph | ddhp | input mismatch |
|---|---|---|---|---|---|---|---|---|---|---|---|
| pT | near | 3e-01 / 1e-09 | 0 / 0 | 1e+00 / 3e-05 | 4e-01 / 1e-06 | 2e-01 / 3e-07 | 9e-01 / 3e-05 | 1e+00 / 3e-05 | 3e-01 / 5e-07 | 4e-01 / 4e-07 | 4e-02 / 6e-11 |
| pT | far | 2e-02 / 7e-11 | 0 / 0 | 1e-01 / 2e-09 | 2e-02 / 2e-09 | 8e-03 / 1e-09 | 9e-02 / 4e-09 | 1e-01 / 4e-09 | 1e-02 / 2e-09 | 3e-02 / 2e-09 | 4e-03 / 9e-11 |
| ps | near | 5e-05 / 2e-07 | 6e-05 / 2e-09 | 3e-03 / 4e-05 | 4e-04 / 5e-07 | 2e-04 / 1e-07 | 3e-03 / 4e-05 | 3e-03 / 4e-05 | 3e-04 / 2e-07 | 1e-04 / 6e-07 | 2e-04 / 7e-09 |
| ps | far | 2e-07 / 1e-09 | 2e-07 / 2e-10 | 3e-06 / 6e-09 | 5e-07 / 9e-10 | 6e-07 / 7e-10 | 3e-06 / 6e-09 | 3e-06 / 9e-09 | 1e-06 / 1e-09 | 8e-07 / 4e-09 | 4e-06 / 8e-13 |

#### R134a

117 p-h test points, 0 with a different phase (one- or two-phase) in the table than in HEOS. Failed calls: A 0, B 0, C 0, TTSE 30, BICUBIC 0.

Maximum relative error against plain HEOS, p-h inputs, cells are A / B / C:

| region | rho | T | cp | cv | a | kappa | beta | ddph | ddhp |
|---|---|---|---|---|---|---|---|---|---|
| liquid dT_sc=0.001 K | 1e-04 / 1e-04 / 6e-08 | 1e-05 / 1e-05 / 7e-09 | 5e-01 / 3e-03 / 2e-06 | 5e+00 / 1e-04 / 5e-08 | 5e-04 / 5e-04 / 2e-07 | 2e+00 / 4e-03 / 2e-06 | 5e-01 / 3e-03 / 2e-06 | 9e-01 / 9e-04 / 4e-07 | 1e-01 / 5e-04 / 2e-07 |
| liquid dT_sc=0.01 K | 6e-05 / 6e-05 / 1e-08 | 7e-06 / 7e-06 / 2e-09 | 2e-01 / 1e-03 / 2e-06 | 3e+00 / 5e-05 / 1e-08 | 1e-04 / 2e-04 / 5e-08 | 1e+00 / 2e-03 / 2e-06 | 2e-01 / 1e-03 / 2e-06 | 7e-01 / 4e-04 / 9e-08 | 7e-02 / 2e-04 / 8e-08 |
| liquid dT_sc=0.1 K | 2e-06 / 2e-06 / 7e-12 | 6e-07 / 6e-07 / 2e-12 | 9e-04 / 5e-06 / 1e-10 | 3e-01 / 5e-07 / 3e-12 | 9e-06 / 6e-06 / 3e-11 | 4e-01 / 2e-05 / 1e-10 | 3e-03 / 1e-05 / 1e-10 | 4e-01 / 1e-05 / 6e-11 | 3e-03 / 6e-06 / 4e-11 |
| liquid dT_sc=1 K | 4e-07 / 4e-07 / 1e-08 | 4e-07 / 4e-07 / 8e-10 | 2e-04 / 9e-07 / 9e-08 | 8e-03 / 1e-07 / 6e-09 | 7e-06 / 2e-06 / 4e-08 | 2e-02 / 4e-06 / 2e-07 | 2e-04 / 3e-06 / 1e-07 | 4e-02 / 3e-06 / 7e-08 | 2e-04 / 2e-06 / 3e-08 |
| liquid dT_sc=10 K | 6e-07 / 6e-07 / 2e-09 | 6e-06 / 6e-06 / 5e-10 | 2e-03 / 5e-06 / 3e-09 | 2e-01 / 2e-06 / 5e-10 | 4e-05 / 9e-06 / 5e-09 | 3e-01 / 2e-05 / 1e-08 | 2e-03 / 2e-05 / 9e-09 | 2e-01 / 2e-05 / 8e-09 | 4e-04 / 2e-05 / 4e-09 |
| vapour dT_sh=0.001 K | 1e-05 / 1e-05 / 5e-10 | 2e-06 / 2e-06 / 2e-10 | 1e-01 / 5e-05 / 2e-09 | 1e+00 / 1e-06 / 2e-09 | 7e-06 / 2e-06 / 2e-10 | 1e-01 / 6e-05 / 2e-09 | 1e-01 / 5e-05 / 2e-09 | 2e-02 / 4e-06 / 7e-10 | 2e-02 / 1e-05 / 3e-09 |
| vapour dT_sh=0.01 K | 9e-06 / 9e-06 / 2e-09 | 1e-06 / 1e-06 / 2e-10 | 4e-02 / 8e-07 / 2e-08 | 3e-02 / 6e-07 / 2e-09 | 3e-06 / 7e-07 / 8e-10 | 2e-02 / 8e-06 / 2e-08 | 5e-02 / 3e-06 / 2e-08 | 4e-03 / 2e-06 / 2e-09 | 2e-02 / 1e-05 / 3e-09 |
| vapour dT_sh=0.1 K | 3e-06 / 3e-06 / 5e-10 | 4e-07 / 4e-07 / 2e-10 | 1e-03 / 6e-07 / 6e-07 | 4e-03 / 1e-07 / 3e-09 | 1e-06 / 2e-07 / 6e-09 | 8e-04 / 2e-06 / 6e-07 | 8e-03 / 8e-07 / 6e-07 | 7e-04 / 5e-07 / 1e-08 | 1e-02 / 4e-06 / 2e-08 |
| vapour dT_sh=1 K | 7e-09 / 7e-09 / 4e-10 | 1e-10 / 1e-10 / 1e-11 | 4e-07 / 3e-08 / 2e-09 | 6e-06 / 2e-09 / 1e-09 | 4e-09 / 3e-09 / 2e-10 | 1e-06 / 3e-08 / 2e-09 | 3e-07 / 3e-08 / 3e-09 | 8e-06 / 7e-09 / 4e-10 | 2e-07 / 1e-08 / 9e-10 |
| vapour dT_sh=10 K | 5e-09 / 5e-09 / 6e-10 | 4e-10 / 4e-10 / 5e-10 | 3e-08 / 5e-09 / 9e-11 | 2e-06 / 8e-10 / 2e-10 | 2e-09 / 2e-09 / 3e-10 | 2e-06 / 4e-09 / 1e-10 | 6e-08 / 1e-08 / 1e-09 | 4e-06 / 4e-09 / 7e-10 | 5e-08 / 1e-08 / 2e-09 |
| two-phase x=0.001 | 5e-13 / 2e-14 / 1e-11 | 0 / 0 / 3e-14 | 5e-01 / 8e-15 / 2e-11 | 7e+00 / 0 / 2e-13 | 6e-04 / 2e-16 / 8e-13 | - / - / - | - / - / - | 1e-07 / 3e-14 / 3e-11 | 2e-05 / 3e-14 / 3e-11 |
| two-phase x=0.01 | 1e-13 / 4e-15 / 3e-12 | 0 / 0 / 3e-14 | 5e-01 / 8e-15 / 2e-11 | 7e+00 / 2e-16 / 2e-13 | 6e-04 / 1e-16 / 8e-13 | - / - / - | - / - / - | 3e-08 / 7e-15 / 1e-11 | 9e-07 / 7e-15 / 6e-12 |
| two-phase x=0.5 | 4e-15 / 8e-16 / 4e-13 | 0 / 0 / 3e-14 | 4e-01 / 5e-15 / 5e-12 | 2e+00 / 2e-16 / 3e-14 | 3e-04 / 2e-16 / 2e-13 | - / - / - | - / - / - | 4e-08 / 4e-15 / 9e-13 | 6e-09 / 4e-15 / 1e-12 |
| two-phase x=0.99 | 3e-15 / 3e-16 / 3e-13 | 0 / 0 / 3e-14 | 3e-01 / 9e-15 / 2e-11 | 2e+00 / 0 / 3e-13 | 5e-05 / 8e-16 / 4e-13 | - / - / - | - / - / - | 4e-08 / 3e-15 / 1e-11 | 4e-09 / 3e-15 / 7e-13 |
| two-phase x=0.999 | 3e-15 / 4e-16 / 3e-13 | 0 / 0 / 3e-14 | 3e-01 / 9e-15 / 2e-11 | 2e+00 / 0 / 3e-13 | 4e-05 / 8e-16 / 4e-13 | - / - / - | - / - / - | 4e-08 / 4e-15 / 1e-11 | 4e-09 / 2e-15 / 7e-13 |
| supercritical | 2e-08 / 2e-08 / 9e-10 | 4e-09 / 4e-09 / 4e-10 | 2e-06 / 3e-08 / 5e-10 | 7e-06 / 3e-09 / 6e-10 | 2e-08 / 5e-08 / 1e-09 | 2e-06 / 2e-07 / 2e-09 | 5e-06 / 1e-07 / 4e-09 | 3e-06 / 1e-07 / 2e-09 | 4e-06 / 6e-08 / 5e-09 |

Consistency with the inputs, max |p - p_in|/p_in and max |h - h_in| in J/kg, B / C:

| region | p rel. | h J/kg |
|---|---|---|
| liquid dT_sc=0.001 K | 7e-03 / 1e-07 | 1.7 / 0.00023 |
| liquid dT_sc=0.01 K | 6e-03 / 5e-08 | 0.8 / 4.9e-05 |
| liquid dT_sc=0.1 K | 4e-03 / 8e-09 | 0.26 / 3.2e-07 |
| liquid dT_sc=1 K | 4e-03 / 1e-08 | 0.065 / 2.4e-07 |
| liquid dT_sc=10 K | 8e-03 / 1e-08 | 2.7 / 2.4e-06 |
| vapour dT_sh=0.001 K | 1e-05 / 6e-11 | 0.46 / 3.1e-06 |
| vapour dT_sh=0.01 K | 8e-06 / 1e-11 | 0.3 / 6.6e-07 |
| vapour dT_sh=0.1 K | 3e-06 / 1e-12 | 0.065 / 3.9e-09 |
| vapour dT_sh=1 K | 2e-09 / 2e-16 | 0.00039 / 1.2e-10 |
| vapour dT_sh=10 K | 2e-09 / 4e-16 | 0.00028 / 5.8e-11 |
| two-phase x=0.001 | 0 / 0 | 0 / 0 |
| two-phase x=0.01 | 0 / 0 | 0 / 0 |
| two-phase x=0.5 | 0 / 0 | 0 / 0 |
| two-phase x=0.99 | 0 / 0 | 0 / 0 |
| two-phase x=0.999 | 0 / 0 | 0 / 0 |
| supercritical | 2e-07 / 1e-14 | 0.001 / 1.2e-10 |

Saturation properties at p, maximum relative error, A / B / C:

| T_sat | rho_l | rho_v | h_l | h_v | dTp | ddldp | dhldp | ddvdp | dhvdp |
|---|---|---|---|---|---|---|---|---|---|
| 0 / 0 / 3e-14 | 0 / 0 / 8e-13 | 0 / 0 / 1e-12 | 2e-15 / 3e-16 / 1e-13 | 1e-15 / 0 / 2e-13 | 7e-10 / 1e-15 / 1e-12 | 2e-07 / 7e-15 / 2e-11 | 2e-07 / 4e-15 / 2e-11 | 2e-07 / 3e-15 / 2e-11 | 2e-07 / 5e-15 / 2e-11 |

Saturation pressure consistency, max |p(T_sat, rho) - p|/p: B 3e-12, C 7e-12

Single-phase p-T and p-s inputs, maximum relative error, B / C. For p-T, C is a converged Newton iteration from the table guess. near: within 0.1 K of saturation, far: the other single-phase points. Failed calls p-T: B 0, C 0; p-s: B 0, C 0.

| inputs | region | rho | T | cp | cv | a | kappa | beta | ddph | ddhp | input mismatch |
|---|---|---|---|---|---|---|---|---|---|---|---|
| pT | near | 7e-01 / 3e-09 | 0 / 0 | 8e-01 / 2e-06 | 5e-02 / 1e-08 | 4e-01 / 5e-08 | 1e+01 / 2e-06 | 8e-01 / 2e-06 | 1e+00 / 9e-08 | 6e-01 / 8e-08 | 1e-02 / 8e-11 |
| pT | far | 7e-06 / 3e-11 | 0 / 0 | 1e-05 / 3e-08 | 1e-06 / 1e-09 | 2e-05 / 5e-09 | 5e-05 / 4e-08 | 4e-05 / 3e-08 | 3e-05 / 1e-08 | 2e-05 / 8e-09 | 4e-05 / 9e-11 |
| ps | near | 3e-04 / 2e-07 | 3e-05 / 3e-08 | 7e-03 / 5e-06 | 2e-04 / 2e-07 | 1e-03 / 9e-07 | 8e-03 / 7e-06 | 7e-03 / 6e-06 | 2e-03 / 2e-06 | 1e-03 / 1e-06 | 9e-03 / 4e-07 |
| ps | far | 1e-06 / 1e-09 | 6e-07 / 3e-10 | 4e-06 / 2e-08 | 4e-07 / 1e-09 | 5e-06 / 3e-09 | 1e-05 / 2e-08 | 1e-05 / 2e-08 | 9e-06 / 6e-09 | 6e-06 / 6e-09 | 2e-02 / 9e-08 |

#### Random points, variant C

| fluid | Newton | worst rel. error | property | where | max p mismatch | max h mismatch J/kg | failed |
|---|---|---|---|---|---|---|---|
| Water | one step | 8e-07 | kappa | p_r=0.9445 T=642.3370 K | 2e-07 | 0.042 | 0 |
| Water | converged, max 5 | 3e-07 | beta | p_r=1.1296 T=658.4540 K | 1e-04 | 0.0023 | 0 |
| CO2 | one step | 3e-05 | cp | p_r=0.9797 T=303.2356 K | 6e-07 | 0.19 | 0 |
| CO2 | converged, max 5 | 3e-05 | beta | p_r=0.9974 T=304.0371 K | 3e-07 | 0.0005 | 0 |
| R134a | one step | 2e-06 | kappa | p_r=0.9511 T=371.7370 K | 6e-05 | 0.00026 | 0 |
| R134a | converged, max 5 | 1e-07 | kappa | p_r=1.1334 T=377.2047 K | 6e-05 | 0.00029 | 0 |

## Timing, microseconds per call

| fluid | transport | region | HEOS | TTSE | BICUBIC | A | B | C | C converged | C, table transport |
|---|---|---|---|---|---|---|---|---|---|---|
| Water | no | single-phase, within 0.1 K of saturation | 139.92 | 0.71 | 7.58 | 4.37 | 4.22 | 7.71 | 9.43 |  |
| Water | no | single-phase, other | 206.01 | 0.66 | 7.24 | 3.22 | 4.13 | 8.10 | 6.17 |  |
| Water | no | two-phase | 7.17 | 0.84 (3 failed) | 1.23 (3 failed) | 10.67 | 7.77 | 7.74 | 7.69 |  |
| Water | yes | single-phase, within 0.1 K of saturation | 153.29 | 1.73 (18 failed) | 7.49 | 4.54 | 16.63 | 19.49 | 21.42 | 7.83 |
| Water | yes | single-phase, other | 213.98 | 1.07 (7 failed) | 7.15 | 3.49 | 15.92 | 19.60 | 18.35 | 8.08 |
| Water | yes | two-phase | 7.46 | 0.86 (3 failed) | 1.27 (3 failed) | 12.11 | 7.89 | 8.82 | 8.04 | 7.87 |
| CO2 | no | single-phase, within 0.1 K of saturation | 143.84 | 0.71 | 7.39 | 4.21 | 4.28 | 7.75 | 9.23 |  |
| CO2 | no | single-phase, other | 202.55 | 0.66 | 7.15 | 2.96 | 4.10 | 7.57 | 6.97 |  |
| CO2 | no | two-phase | 7.35 | 0.64 | 0.64 | 10.53 | 7.93 | 7.92 | 7.92 |  |
| CO2 | yes | single-phase, within 0.1 K of saturation | 149.35 | 1.66 (12 failed) | 7.42 | 4.49 | 10.81 | 14.26 | 15.73 | 8.11 |
| CO2 | yes | single-phase, other | 207.63 | 1.09 (6 failed) | 7.17 | 3.25 | 10.55 | 14.09 | 13.43 | 7.89 |
| CO2 | yes | two-phase | 7.30 | 0.64 | 0.64 | 11.00 | 7.90 | 7.92 | 7.95 | 7.93 |
| R134a | no | single-phase, within 0.1 K of saturation | 32.68 | 0.71 | 7.45 | 4.27 | 2.06 | 3.33 | 3.57 |  |
| R134a | no | single-phase, other | 43.77 | 0.68 | 7.10 | 3.03 | 1.87 | 3.12 | 2.79 |  |
| R134a | no | two-phase | 2.91 | 0.64 | 0.64 | 10.54 | 3.48 | 3.49 | 3.47 |  |
| R134a | yes | single-phase, within 0.1 K of saturation | 34.89 | 2.04 (24 failed) | 7.49 | 4.59 | 4.02 | 5.32 | 5.54 | 3.61 |
| R134a | yes | single-phase, other | 46.56 | 1.05 (6 failed) | 7.25 | 3.37 | 3.83 | 5.12 | 4.76 | 3.45 |
| R134a | yes | two-phase | 3.01 | 0.65 | 0.64 | 11.25 | 3.48 | 3.49 | 3.47 | 3.50 |

p-T inputs, single-phase, microseconds per call

| fluid | transport | HEOS | C, update_with_guesses | C, own Newton |
|---|---|---|---|---|
| Water | no | 25.17 | 19.26 | 10.33 |
| Water | yes | 35.42 | 31.32 | 22.15 |
| CO2 | no | 22.84 | 20.05 | 10.51 |
| CO2 | yes | 29.67 | 26.62 | 17.08 |
| R134a | no | 5.98 | 5.15 | 3.22 |
| R134a | yes | 7.94 | 7.32 | 5.26 |

setSat_p, microseconds per call

| fluid | HEOS | B |
|---|---|---|
| Water | 18.76 | 7.20 |
| CO2 | 19.38 | 7.34 |
| R134a | 5.87 | 3.14 |
