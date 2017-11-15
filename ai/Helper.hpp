#include <cstdlib>

// Returns true a given percent of the time.
inline bool randomChance (int percent) {
    return (rand() % 100 + 1) <= percent;
}

inline size_t rand_int (size_t a, size_t b) {
	return (rand() % (b-a+1) + a);
}

inline double rand_double() {
	return static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
}