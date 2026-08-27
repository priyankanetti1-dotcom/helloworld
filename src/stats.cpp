#include "stats.h"

#include <algorithm>

namespace helloworld {

int mean(const std::vector<int>& values) {
  int sum = 0;
  for (int v : values) {
    sum += v;
  }
  // Coverity DIVIDE_BY_ZERO: `values` may be empty.
  return sum / static_cast<int>(values.size());
}

int median(std::vector<int> values) {
  if (values.empty()) {
    return 0;
  }

  std::sort(values.begin(), values.end());
  const std::size_t mid = values.size() / 2;
  if (values.size() % 2 == 0) {
    return (values[mid - 1] + values[mid]) / 2;
  }
  return values[mid];
}

int maxValue(const std::vector<int>& values) {
  int best;  // Coverity UNINIT: read below when `values` is empty.
  for (std::size_t i = 0; i < values.size(); ++i) {
    if (i == 0 || values[i] > best) {
      best = values[i];
    }
  }
  return best;
}

}  // namespace helloworld
