// Test of the state caches in the CoolProp solver (see Sources/statecache.h).
//
// The same states are computed with the cache switched off ("R134a|cache_size=0")
// and switched on ("R134a"). Every field of the returned records must be
// bit for bit identical, both for the first call (cache miss) and for a
// repeated call (cache hit). At the end, a sweep that imitates a numerical
// Jacobian prints the run time with different cache sizes.
//
// Build and run:
//   cmake --build . --target state_cache_test
//   ./state_cache_test
// The program returns 0 if all checks pass.

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include "externalmedialib.h"

// The library reports errors through the Modelica utility functions,
// outside of a Modelica tool they have to be provided here.
extern "C" void ModelicaError(const char *msg) {
	printf("ModelicaError: %s\n", msg);
	exit(2);
}
extern "C" void ModelicaWarning(const char *msg) {
	printf("ModelicaWarning: %s\n", msg);
}

static const char *MEDIUM = "R134a";
static const char *LIBRARY = "CoolProp";
static const char *CACHED = "R134a";
static const char *UNCACHED = "R134a|cache_size=0";
static int failures = 0;

// Set all bytes of the records to zero. ExternalThermodynamicState has
// padding after the int phase, the solver never writes it, so without this
// checkSame could report a difference in uninitialised padding bytes.
template <class Record>
static void clear(Record &a) {
	memset(&a, 0, sizeof(Record));
}
template <class Record, class... More>
static void clear(Record &a, More &... more) {
	clear(a);
	clear(more...);
}

// Compare two records byte by byte
template <class Record>
static void checkSame(const char *what, double x1, double x2, const Record &a, const Record &b) {
	if (memcmp(&a, &b, sizeof(Record)) != 0) {
		printf("FAILED: %s differs for inputs (%g, %g)\n", what, x1, x2);
		failures++;
	}
}

// Compute a state without cache, then twice with cache (miss and hit).
// Another state is computed in between, so that the hit does not simply
// return the last result.
template <class SetState>
static void checkFlavour(const char *what, double x1, double x2, SetState setState) {
	ExternalThermodynamicState uncached, miss, other, hit;
	clear(uncached, miss, other, hit);
	setState(x1, x2, &uncached, UNCACHED);
	setState(x1, x2, &miss, CACHED);
	TwoPhaseMedium_setState_ph_C_impl(1.234e5, 4.0e5, 0, &other, MEDIUM, LIBRARY, CACHED);
	setState(x1, x2, &hit, CACHED);
	checkSame(what, x1, x2, uncached, miss);
	checkSame(what, x1, x2, uncached, hit);
}

int main() {
	// Liquid, two-phase, vapour, supercritical
	double p_Pa[] = {3e5, 3e5, 3e5, 6e6, 1e6};
	double h_J_kg[] = {2.0e5, 3.0e5, 4.2e5, 4.5e5, 4.4e5};
	for (int k = 0; k < 5; k++) {
		ExternalThermodynamicState ref;
		TwoPhaseMedium_setState_ph_C_impl(p_Pa[k], h_J_kg[k], 0, &ref, MEDIUM, LIBRARY, UNCACHED);

		checkFlavour("setState_ph", ref.p, ref.h, [](double p, double h, ExternalThermodynamicState *st, const char *sub) {
			TwoPhaseMedium_setState_ph_C_impl(p, h, 0, st, MEDIUM, LIBRARY, sub); });
		if (ref.phase == 1)  // T and p are not independent in the two-phase region
			checkFlavour("setState_pT", ref.p, ref.T, [](double p, double T, ExternalThermodynamicState *st, const char *sub) {
				TwoPhaseMedium_setState_pT_C_impl(p, T, st, MEDIUM, LIBRARY, sub); });
		checkFlavour("setState_dT", ref.d, ref.T, [](double d, double T, ExternalThermodynamicState *st, const char *sub) {
			TwoPhaseMedium_setState_dT_C_impl(d, T, 0, st, MEDIUM, LIBRARY, sub); });
		checkFlavour("setState_ps", ref.p, ref.s, [](double p, double s, ExternalThermodynamicState *st, const char *sub) {
			TwoPhaseMedium_setState_ps_C_impl(p, s, 0, st, MEDIUM, LIBRARY, sub); });
		checkFlavour("setState_hs", ref.h, ref.s, [](double h, double s, ExternalThermodynamicState *st, const char *sub) {
			TwoPhaseMedium_setState_hs_C_impl(h, s, 0, st, MEDIUM, LIBRARY, sub); });
	}

	// Saturation properties and bubble states, subcritical pressures only
	double p_sat_Pa[] = {1e5, 1e6, 3e6};
	for (double p : p_sat_Pa) {
		ExternalSaturationProperties uncached, miss, other, hit;
		clear(uncached, miss, other, hit);
		TwoPhaseMedium_setSat_p_C_impl(p, &uncached, MEDIUM, LIBRARY, UNCACHED);
		TwoPhaseMedium_setSat_p_C_impl(p, &miss, MEDIUM, LIBRARY, CACHED);
		TwoPhaseMedium_setSat_p_C_impl(0.9 * p, &other, MEDIUM, LIBRARY, CACHED);
		TwoPhaseMedium_setSat_p_C_impl(p, &hit, MEDIUM, LIBRARY, CACHED);
		checkSame("setSat_p", p, 0.0, uncached, miss);
		checkSame("setSat_p", p, 0.0, uncached, hit);

		double T = uncached.Tsat;
		TwoPhaseMedium_setSat_T_C_impl(T, &uncached, MEDIUM, LIBRARY, UNCACHED);
		TwoPhaseMedium_setSat_T_C_impl(T, &miss, MEDIUM, LIBRARY, CACHED);
		TwoPhaseMedium_setSat_T_C_impl(1.01 * T, &other, MEDIUM, LIBRARY, CACHED);
		TwoPhaseMedium_setSat_T_C_impl(T, &hit, MEDIUM, LIBRARY, CACHED);
		checkSame("setSat_T", T, 0.0, uncached, miss);
		checkSame("setSat_T", T, 0.0, uncached, hit);

		ExternalThermodynamicState bubble_uncached, bubble_miss, bubble_hit;
		clear(bubble_uncached, bubble_miss, bubble_hit);
		TwoPhaseMedium_setBubbleState_C_impl(&uncached, 1, &bubble_uncached, MEDIUM, LIBRARY, UNCACHED);
		TwoPhaseMedium_setBubbleState_C_impl(&uncached, 1, &bubble_miss, MEDIUM, LIBRARY, CACHED);
		TwoPhaseMedium_setBubbleState_C_impl(&uncached, 1, &bubble_hit, MEDIUM, LIBRARY, CACHED);
		checkSame("setBubbleState", p, 0.0, bubble_uncached, bubble_miss);
		checkSame("setBubbleState", p, 0.0, bubble_uncached, bubble_hit);
	}

	// The input pair is part of the key: a pT entry must not be returned
	// for a dT call with the same numbers (20 Pa or 20 kg/m3, 300 K).
	{
		ExternalThermodynamicState state_pT, state_dT, state_dT_uncached;
		clear(state_pT, state_dT, state_dT_uncached);
		TwoPhaseMedium_setState_pT_C_impl(20.0, 300.0, &state_pT, MEDIUM, LIBRARY, CACHED);
		TwoPhaseMedium_setState_dT_C_impl(20.0, 300.0, 0, &state_dT, MEDIUM, LIBRARY, CACHED);
		TwoPhaseMedium_setState_dT_C_impl(20.0, 300.0, 0, &state_dT_uncached, MEDIUM, LIBRARY, UNCACHED);
		checkSame("setState_dT after setState_pT", 20.0, 300.0, state_dT_uncached, state_dT);
		if (state_pT.d == state_dT.d) {
			printf("FAILED: setState_dT returned the setState_pT entry\n");
			failures++;
		}
	}

	// Sweep that imitates a numerical Jacobian: n_states states are evaluated
	// n_states + 1 times, each time with one of them slightly perturbed.
	const char *variants[] = {"R134a|cache_size=0", "R134a|cache_size=64", "R134a", "R134a|cache_size=4096"};
	for (int n_states : {10, 100, 500}) {
		std::vector<double> p_Pa_sweep(n_states), h_J_kg_sweep(n_states);
		for (int i = 0; i < n_states; i++) {
			p_Pa_sweep[i] = 2e5 + 1e3 * i;
			h_J_kg_sweep[i] = 2.0e5 + 2.5e5 * i / n_states;
		}
		for (const char *variant : variants) {
			ExternalThermodynamicState state;
			auto t_start = std::chrono::steady_clock::now();
			for (int eval = 0; eval <= n_states; eval++) {
				for (int i = 0; i < n_states; i++) {
					double h = h_J_kg_sweep[i];
					if (eval > 0 && i == eval - 1) h *= 1.0 + 1e-7;
					TwoPhaseMedium_setState_ph_C_impl(p_Pa_sweep[i], h, 0, &state, MEDIUM, LIBRARY, variant);
				}
			}
			double t_s = std::chrono::duration<double>(std::chrono::steady_clock::now() - t_start).count();
			printf("%4d states, %-22s %8.3f s\n", n_states, variant, t_s);
		}
	}

	printf("%d failures\n", failures);
	return failures == 0 ? 0 : 1;
}
