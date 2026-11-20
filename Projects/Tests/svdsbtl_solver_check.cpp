/*
 * Check of the enable_SVDSBTL option of the CoolProp solver.
 *
 * Calls the ExternalMedia C interface twice for every test point, once
 * for the plain fluid ("Water") and once with the SVDSBTL option
 * ("Water|enable_SVDSBTL=1"), and reports the largest relative
 * difference of every output, the number of errors raised and the time
 * per call. The test points are the same as in svdsbtl_prototype.py:
 * subcooled liquid and superheated vapour at 0.001 to 10 K from the
 * saturation line, two-phase states and supercritical states.
 *
 * Build on Linux against the ExternalMedia shared library, for example:
 *
 *   g++ -O2 -std=c++17 -rdynamic svdsbtl_solver_check.cpp \
 *       -I../Sources -L<build> -lExternalMediaLib -o svdsbtl_solver_check
 *
 * -rdynamic exports ModelicaError and ModelicaWarning from this program,
 * the library calls them to report errors.
 */

#include "externalmedialib.h"

#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

// Error and warning counters, the library calls these functions.
// ModelicaError must not return (it is declared noreturn), so it throws
// an exception that the calls below catch. The type is not derived from
// std::exception, so the catch blocks inside the library do not take it.
struct ModelicaErrorRaised
{};
static bool g_verbose = true;
static int n_errors = 0;
static std::string last_error;
extern "C" void ModelicaError(const char* msg) {
    n_errors++;
    last_error = msg;
    throw ModelicaErrorRaised();
}
extern "C" void ModelicaWarning(const char* msg) {
    std::printf("warning: %s\n", msg);
}
extern "C" void ModelicaFormatError(const char* fmt, ...) {
    n_errors++;
    last_error = fmt;
    throw ModelicaErrorRaised();
}
extern "C" void ModelicaFormatMessage(const char*, ...) {}
extern "C" void ModelicaMessage(const char*) {}

// Call a library function, an error raised by it is counted and skipped
#define CALL(expr)                  \
    do {                            \
        try {                       \
            expr;                   \
        } catch (ModelicaErrorRaised&) { \
            if (g_verbose) std::printf("error in %s: %s\n", #expr, last_error.c_str()); \
        }                           \
    } while (0)

// Run a call of the plain solver, true if it raised no error
#define PLAIN_OK(expr) (([&]() {     \
    const int e0_ = n_errors;        \
    g_verbose = false;               \
    CALL(expr);                      \
    g_verbose = true;                \
    const bool ok_ = (n_errors == e0_); \
    n_errors = e0_;                  \
    return ok_;                      \
})())

static const char* MEDIUM = "medium";
static const char* LIB = "CoolProp";

// Largest relative difference per output name
struct MaxErr
{
    std::vector<std::string> names;
    std::vector<double> err;
    std::vector<std::string> where;
    void add(const std::string& name, double v, double ref, const std::string& w) {
        double e;
        if (std::isnan(v) && std::isnan(ref))
            e = 0;
        else if (std::isnan(v) || std::isnan(ref))
            e = INFINITY;
        else
            e = std::abs(v - ref) / (std::abs(ref) > 0 ? std::abs(ref) : 1.0);
        for (size_t i = 0; i < names.size(); i++)
            if (names[i] == name) {
                if (e > err[i]) {
                    err[i] = e;
                    where[i] = w;
                }
                return;
            }
        names.push_back(name);
        err.push_back(e);
        where.push_back(w);
    }
    void print(const char* title) const {
        std::printf("\n%s\n", title);
        for (size_t i = 0; i < names.size(); i++)
            std::printf("  %-7s %9.1e  %s\n", names[i].c_str(), err[i], err[i] > 0 ? where[i].c_str() : "");
    }
    double worst() const {
        double w = 0;
        for (double e : err)
            w = std::max(w, e);
        return w;
    }
};

static void compare_state(MaxErr& m, const ExternalThermodynamicState& a, const ExternalThermodynamicState& r,
                          const std::string& w) {
    m.add("T", a.T, r.T, w);
    m.add("p", a.p, r.p, w);
    m.add("d", a.d, r.d, w);
    m.add("h", a.h, r.h, w);
    m.add("s", a.s, r.s, w);
    m.add("cp", a.cp, r.cp, w);
    m.add("cv", a.cv, r.cv, w);
    m.add("a", a.a, r.a, w);
    m.add("kappa", a.kappa, r.kappa, w);
    m.add("beta", a.beta, r.beta, w);
    m.add("ddph", a.ddph, r.ddph, w);
    m.add("ddhp", a.ddhp, r.ddhp, w);
    m.add("eta", a.eta, r.eta, w);
    m.add("lambda", a.lambda, r.lambda, w);
    m.add("phase", a.phase, r.phase, w);
}

static void compare_sat(MaxErr& m, const ExternalSaturationProperties& a, const ExternalSaturationProperties& r,
                        const std::string& w) {
    m.add("Tsat", a.Tsat, r.Tsat, w);
    m.add("psat", a.psat, r.psat, w);
    m.add("dl", a.dl, r.dl, w);
    m.add("dv", a.dv, r.dv, w);
    m.add("hl", a.hl, r.hl, w);
    m.add("hv", a.hv, r.hv, w);
    m.add("sl", a.sl, r.sl, w);
    m.add("sv", a.sv, r.sv, w);
    m.add("dTp", a.dTp, r.dTp, w);
    m.add("ddldp", a.ddldp, r.ddldp, w);
    m.add("dhldp", a.dhldp, r.dhldp, w);
    m.add("ddvdp", a.ddvdp, r.ddvdp, w);
    m.add("dhvdp", a.dhvdp, r.dhvdp, w);
    m.add("sigma", a.sigma, r.sigma, w);
}

struct Pt
{
    double p, h;
    std::string where;
};

static double us_per_call(const std::vector<Pt>& pts, const char* sub) {
    ExternalThermodynamicState st;
    double best = 1e30;
    for (int rep = 0; rep < 3; rep++) {
        const auto t0 = std::chrono::steady_clock::now();
        for (int k = 0; k < 10; k++)
            for (const Pt& q : pts)
                CALL(TwoPhaseMedium_setState_ph_C_impl(q.p, q.h, 0, &st, MEDIUM, LIB, sub));
        const auto t1 = std::chrono::steady_clock::now();
        best = std::min(best, std::chrono::duration<double, std::micro>(t1 - t0).count() / (10.0 * pts.size()));
    }
    return best;
}

static double check_fluid(const std::string& fluid, const std::string& extra_in) {
    // The state cache is switched off, so that every call is computed and
    // the timing loop below does not measure cache hits
    const std::string extra = extra_in + "|cache_size=0";
    const std::string ref = fluid + extra;
    const std::string svd = fluid + "|enable_SVDSBTL=1" + extra;
    const char* r = ref.c_str();
    const char* v = svd.c_str();
    std::printf("\n==== %s against %s ====\n", v, r);
    n_errors = 0;

    double pc = NAN, Tc = NAN;
    CALL(pc = TwoPhaseMedium_getCriticalPressure_C_impl(MEDIUM, LIB, r));
    CALL(Tc = TwoPhaseMedium_getCriticalTemperature_C_impl(MEDIUM, LIB, r));
    // Construct the SVDSBTL solver first, so its construction time is visible
    const auto t0 = std::chrono::steady_clock::now();
    CALL(TwoPhaseMedium_getCriticalPressure_C_impl(MEDIUM, LIB, v));
    const double t_build_s = std::chrono::duration<double>(std::chrono::steady_clock::now() - t0).count();
    std::printf("construction of the SVDSBTL solver: %.2f s\n", t_build_s);

    // Saturation states and test points
    MaxErr m_sat, m_ph1, m_ph2, m_pT, m_ps, m_dT, m_hs, m_bd;
    std::vector<Pt> single, twoph;
    ExternalSaturationProperties sr, sv;
    ExternalThermodynamicState a, b;
    for (double pr : {0.001, 0.01, 0.05, 0.2, 0.5, 0.8, 0.95, 0.99}) {
        const double p = pr * pc;
        char w[200];
        std::snprintf(w, sizeof(w), "p/p_c=%g", pr);
        // Skip pressures where the plain solver fails (below the triple point)
        if (!PLAIN_OK(TwoPhaseMedium_setSat_p_C_impl(p, &sr, MEDIUM, LIB, r))) continue;
        if (!PLAIN_OK(TwoPhaseMedium_setBubbleState_C_impl(&sr, 1, &a, MEDIUM, LIB, r))) continue;
        if (!PLAIN_OK(TwoPhaseMedium_setDewState_C_impl(&sr, 1, &a, MEDIUM, LIB, r))) continue;
        CALL(TwoPhaseMedium_setSat_p_C_impl(p, &sv, MEDIUM, LIB, v));
        compare_sat(m_sat, sv, sr, std::string("setSat_p ") + w);
        // setSat_T at the saturation temperature
        ExternalSaturationProperties tr, tv;
        CALL(TwoPhaseMedium_setSat_T_C_impl(sr.Tsat, &tr, MEDIUM, LIB, r));
        CALL(TwoPhaseMedium_setSat_T_C_impl(sr.Tsat, &tv, MEDIUM, LIB, v));
        compare_sat(m_sat, tv, tr, std::string("setSat_T ") + w);
        // Bubble and dew states (single-phase and two-phase side)
        for (int ph : {0, 1, 2}) {
            CALL(TwoPhaseMedium_setBubbleState_C_impl(&sr, ph, &a, MEDIUM, LIB, r));
            CALL(TwoPhaseMedium_setBubbleState_C_impl(&sv, ph, &b, MEDIUM, LIB, v));
            compare_state(m_bd, b, a, std::string("bubble ") + w);
            CALL(TwoPhaseMedium_setDewState_C_impl(&sr, ph, &a, MEDIUM, LIB, r));
            CALL(TwoPhaseMedium_setDewState_C_impl(&sv, ph, &b, MEDIUM, LIB, v));
            compare_state(m_bd, b, a, std::string("dew ") + w);
        }
        // Single-phase points at dT from the saturation line, given as p-T
        for (double dT : {0.001, 0.01, 0.1, 1.0, 10.0}) {
            for (int side : {-1, 1}) {
                const double T = sr.Tsat + side * dT;
                char ww[200];
                std::snprintf(ww, sizeof(ww), "%s %s %g K", w, side < 0 ? "liquid" : "vapour", dT);
                // Skip points where the plain solver fails (below the triple point)
                if (!PLAIN_OK(TwoPhaseMedium_setState_pT_C_impl(p, T, &a, MEDIUM, LIB, r))) continue;
                {
                    ExternalThermodynamicState c;
                    if (!PLAIN_OK(TwoPhaseMedium_setState_ph_C_impl(p, a.h, 0, &c, MEDIUM, LIB, r))) continue;
                }
                CALL(TwoPhaseMedium_setState_pT_C_impl(p, T, &b, MEDIUM, LIB, v));
                compare_state(m_pT, b, a, ww);
                single.push_back({p, a.h, ww});
                const double h = a.h, s = a.s, d = a.d;
                CALL(TwoPhaseMedium_setState_ph_C_impl(p, h, 0, &a, MEDIUM, LIB, r));
                CALL(TwoPhaseMedium_setState_ph_C_impl(p, h, 0, &b, MEDIUM, LIB, v));
                compare_state(m_ph1, b, a, ww);
                CALL(TwoPhaseMedium_setState_ps_C_impl(p, s, 0, &a, MEDIUM, LIB, r));
                CALL(TwoPhaseMedium_setState_ps_C_impl(p, s, 0, &b, MEDIUM, LIB, v));
                compare_state(m_ps, b, a, ww);
                CALL(TwoPhaseMedium_setState_dT_C_impl(d, T, 0, &a, MEDIUM, LIB, r));
                CALL(TwoPhaseMedium_setState_dT_C_impl(d, T, 0, &b, MEDIUM, LIB, v));
                compare_state(m_dT, b, a, ww);
                CALL(TwoPhaseMedium_setState_hs_C_impl(h, s, 0, &a, MEDIUM, LIB, r));
                CALL(TwoPhaseMedium_setState_hs_C_impl(h, s, 0, &b, MEDIUM, LIB, v));
                compare_state(m_hs, b, a, ww);
            }
        }
        // Two-phase points
        for (double x : {0.001, 0.01, 0.5, 0.99, 0.999}) {
            const double h = sr.hl + x * (sr.hv - sr.hl);
            char ww[200];
            std::snprintf(ww, sizeof(ww), "%s x=%g", w, x);
            CALL(TwoPhaseMedium_setState_ph_C_impl(p, h, 0, &a, MEDIUM, LIB, r));
            CALL(TwoPhaseMedium_setState_ph_C_impl(p, h, 0, &b, MEDIUM, LIB, v));
            compare_state(m_ph2, b, a, ww);
            twoph.push_back({p, h, ww});
            const double s = a.s;
            CALL(TwoPhaseMedium_setState_ps_C_impl(p, s, 0, &a, MEDIUM, LIB, r));
            CALL(TwoPhaseMedium_setState_ps_C_impl(p, s, 0, &b, MEDIUM, LIB, v));
            compare_state(m_ph2, b, a, std::string("p-s ") + ww);
        }
    }
    // Supercritical points
    for (double pr : {1.2, 2.0, 4.0})
        for (double Tr : {0.8, 0.95, 1.05, 1.2, 2.0}) {
            char ww[200];
            std::snprintf(ww, sizeof(ww), "supercritical p/p_c=%g T/T_c=%g", pr, Tr);
            // Skip points where the plain solver fails (above its maximum temperature)
            if (!PLAIN_OK(TwoPhaseMedium_setState_pT_C_impl(pr * pc, Tr * Tc, &a, MEDIUM, LIB, r))) continue;
            {
                ExternalThermodynamicState c;
                if (!PLAIN_OK(TwoPhaseMedium_setState_ph_C_impl(pr * pc, a.h, 0, &c, MEDIUM, LIB, r))) continue;
            }
            CALL(TwoPhaseMedium_setState_pT_C_impl(pr * pc, Tr * Tc, &b, MEDIUM, LIB, v));
            compare_state(m_pT, b, a, ww);
            single.push_back({pr * pc, a.h, ww});
            const double h = a.h, s = a.s;
            CALL(TwoPhaseMedium_setState_ph_C_impl(pr * pc, h, 0, &a, MEDIUM, LIB, r));
            CALL(TwoPhaseMedium_setState_ph_C_impl(pr * pc, h, 0, &b, MEDIUM, LIB, v));
            compare_state(m_ph1, b, a, ww);
            CALL(TwoPhaseMedium_setState_ps_C_impl(pr * pc, s, 0, &a, MEDIUM, LIB, r));
            CALL(TwoPhaseMedium_setState_ps_C_impl(pr * pc, s, 0, &b, MEDIUM, LIB, v));
            compare_state(m_ps, b, a, ww);
        }
    m_sat.print("setSat_p and setSat_T, largest relative difference");
    m_bd.print("setBubbleState and setDewState");
    m_ph1.print("setState_ph, single-phase");
    m_ph2.print("setState_ph and setState_ps, two-phase");
    m_pT.print("setState_pT");
    m_ps.print("setState_ps, single-phase");
    m_dT.print("setState_dT");
    m_hs.print("setState_hs");
    std::printf("\nerrors raised: %d %s\n", n_errors, n_errors ? last_error.c_str() : "");

    std::printf("\nsetState_ph, microseconds per call: single-phase plain %.1f, SVDSBTL %.1f; "
                "two-phase plain %.1f, SVDSBTL %.1f\n",
                us_per_call(single, r), us_per_call(single, v), us_per_call(twoph, r), us_per_call(twoph, v));
    double worst = 0;
    for (const MaxErr* m : {&m_sat, &m_bd, &m_ph1, &m_ph2, &m_pT, &m_ps, &m_dT, &m_hs})
        worst = std::max(worst, m->worst());
    return n_errors ? INFINITY : worst;
}

// An option string that must raise an error
static void expect_error(const char* sub) {
    n_errors = 0;
    CALL(TwoPhaseMedium_getCriticalPressure_C_impl(MEDIUM, LIB, sub));
    std::printf("%-55s %s\n", sub, n_errors ? ("error as expected: " + last_error).c_str() : "NO ERROR, unexpected");
}

int main(int argc, char** argv) {
    // Line buffering, so the output is complete even if the program stops
    std::setvbuf(stdout, NULL, _IOLBF, 0);
    std::vector<std::string> fluids = {"Water", "CO2", "R134a"};
    if (argc > 1) fluids.assign(argv + 1, argv + argc);
    std::printf("Option checks:\n");
    expect_error("Water|enable_SVDSBTL=1|enable_TTSE=1");
    expect_error("Water|enable_SVDSBTL=1|enable_BICUBIC=1");
    expect_error("Water|enable_SVDSBTL=2");
    expect_error("R32[0.5]&R125[0.5]|enable_SVDSBTL=1");
    expect_error("IF97::Water|enable_SVDSBTL=1");
    std::vector<double> worst;
    for (const std::string& f : fluids)
        worst.push_back(check_fluid(f, ""));
    // The smoothing options work with SVDSBTL, two-phase states use the reference
    worst.push_back(check_fluid(fluids[0], "|twophase_derivsmoothing_xend=0.1"));
    worst.push_back(check_fluid(fluids[0], "|calc_transport=0|enable_EXTTP=0"));
    std::printf("\nWorst relative difference per run:");
    for (double w : worst)
        std::printf(" %.1e", w);
    std::printf("\n");
    return 0;
}
