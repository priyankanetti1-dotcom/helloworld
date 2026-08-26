#ifndef HELLOWORLD_STATS_H
#define HELLOWORLD_STATS_H

#include <vector>

namespace helloworld {

// Arithmetic mean, truncated towards zero.
int mean(const std::vector<int>& values);

// Median value. For an even number of elements the average of the two
// middle elements is returned.
int median(std::vector<int> values);

// Largest element of `values`.
int maxValue(const std::vector<int>& values);

}  // namespace helloworld

#endif  // HELLOWORLD_STATS_H
