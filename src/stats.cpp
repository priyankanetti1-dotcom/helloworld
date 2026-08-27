#include "stats.h"

#include <algorithm>

namespace helloworld {

int mean(const std::vector<int>& values) {
  if (values.empty()) {
    return 0;
  }

  int sum = 0;
  for (int v : values) {
    sum += v;
  }
  return sum / static_cast<int>(values.size());
}

int median(std::vector<int> values) {
  if (values.empty()) {
    return 0;
  }

  std::sort(values.begin(), values.end());
  const std::size_t mid = values.size() / 2;
  if (values.size() % 2 == 0) {
    return static_cast<int>((static_cast<long long>(values[mid - 1]) +
                             static_cast<long long>(values[mid])) /
                            2);
  }
  return values[mid];
}

int maxValue(const std::vector<int>& values) {
  if (values.empty()) {
    return 0;
  }
  return *std::max_element(values.begin(), values.end());
}

}  // namespace helloworld
