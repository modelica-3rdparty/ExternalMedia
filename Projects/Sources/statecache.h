#ifndef STATECACHE_H_
#define STATECACHE_H_

#include <cstring>
#include <vector>

/*! Input pairs that can be stored in a StateCache */
/*!
  The input pair is part of the cache key, so that for example a call to
  setState_ph(1e5, 3e5) and a call to setState_ps(1e5, 3e5) never share a
  cache entry. The saturation functions take a single input, they use the
  second input slot with a value of 0.
*/
enum StateCacheInputs {
	CACHE_PH = 1,
	CACHE_PT,
	CACHE_DT,
	CACHE_PS,
	CACHE_HS,
	CACHE_SAT_P,
	CACHE_SAT_T
};

/*! Small cache for computed property records */
/*!
  A numerical Jacobian re-evaluates every medium state of a model, although
  most of them get exactly the same inputs as in the evaluation before. This
  cache keeps the results of recent calls, so that a repeated call can copy
  the stored record instead of calling the property library again.

  How it works, in short:
  - The cache is a fixed list of slots (a "direct-mapped" cache).
  - The inputs (input pair, x1, x2, phase) are turned into a slot number
    with a hash function, a recipe that scatters similar inputs over all slots.
  - A lookup checks only that one slot. If the stored inputs are bit for bit
    identical to the new inputs, the stored record is returned (a "hit").
  - Otherwise the caller computes the record and stores it in that slot,
    replacing whatever was there before (a "miss").

  Inputs are compared on their exact bit patterns, so the results are the
  same as without the cache, as long as the property calculation itself
  always gives the same result for the same inputs.

  Record is the property struct that is stored, for example
  ExternalThermodynamicState or ExternalSaturationProperties.

  The cache is not thread-safe, just like the solver objects that use it.
*/
template <class Record>
class StateCache {
public:
	StateCache() : _hits(0), _misses(0) {}

	/*! Set the number of slots, 0 switches the cache off. Clears all entries. */
	void resize(int size) {
		_entries.assign(size > 0 ? size : 0, Entry());
	}

	/*! Number of slots, 0 means the cache is switched off */
	int size() const { return (int)_entries.size(); }

	/*! Copy the stored record to result if the inputs are in the cache. */
	/*!
	  Returns true on a hit. On a miss, result is not touched and the
	  caller has to compute the record and call store() afterwards.
	*/
	bool lookup(int inputs, double x1, double x2, int phase, Record *result) {
		if (_entries.empty()) return false;
		const Entry &entry = _entries[slot(inputs, x1, x2, phase)];
		if (entry.valid && entry.inputs == inputs && entry.phase == phase
		    && sameBits(entry.x1, x1) && sameBits(entry.x2, x2)) {
			*result = entry.record;
			_hits++;
			return true;
		}
		_misses++;
		return false;
	}

	/*! Store a computed record, replacing the previous entry of that slot. */
	void store(int inputs, double x1, double x2, int phase, const Record &record) {
		if (_entries.empty()) return;
		Entry &entry = _entries[slot(inputs, x1, x2, phase)];
		entry.valid = true;
		entry.inputs = inputs;
		entry.phase = phase;
		entry.x1 = x1;
		entry.x2 = x2;
		entry.record = record;
	}

	unsigned long long hits() const { return _hits; }
	unsigned long long misses() const { return _misses; }

private:
	struct Entry {
		Entry() : valid(false), inputs(0), phase(0), x1(0.0), x2(0.0) {}
		bool valid;
		int inputs;
		int phase;
		double x1, x2;
		Record record;
	};

	std::vector<Entry> _entries;
	unsigned long long _hits, _misses;

	/*! Raw 64 bits of a double, used for exact comparison and hashing */
	static unsigned long long bits(double x) {
		unsigned long long b;
		memcpy(&b, &x, sizeof(b));
		return b;
	}

	/*! Bitwise comparison, so that NaN matches NaN and -0.0 differs from 0.0 */
	static bool sameBits(double a, double b) {
		return bits(a) == bits(b);
	}

	/*! Slot number for a set of inputs */
	/*!
	  The multiplications with large odd constants and the shifts mix all
	  input bits into the result, so that inputs that differ only in their
	  last digits still end up in different slots.
	*/
	int slot(int inputs, double x1, double x2, int phase) const {
		unsigned long long b1 = bits(x1);
		unsigned long long b2 = bits(x2);
		unsigned long long x = b1 * 0x9E3779B97F4A7C15ULL
		                     ^ (b2 + 0x632BE59BD9B4E019ULL + (b1 << 6) + (b1 >> 2));
		x ^= ((unsigned long long)inputs << 56) ^ ((unsigned long long)(unsigned int)phase << 48);
		x ^= x >> 31;
		x *= 0xBF58476D1CE4E5B9ULL;
		x ^= x >> 29;
		return (int)(x % (unsigned long long)_entries.size());
	}
};

#endif /* STATECACHE_H_ */
