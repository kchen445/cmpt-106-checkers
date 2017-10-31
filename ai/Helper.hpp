#include <cstdlib>

// Returns true a given percent of the time.
inline bool randomChance (int percent) {
    return (rand() % 100 + 1) <= percent;
}
