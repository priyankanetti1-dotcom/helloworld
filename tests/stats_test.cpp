#include "stats.h"

#include <gtest/gtest.h>

namespace {

TEST(StatsTest, MeanOfAPopulatedVector) {
  EXPECT_EQ(3, helloworld::mean({1, 2, 3, 4, 5}));
}

TEST(StatsTest, MeanTruncatesTowardsZero) {
  EXPECT_EQ(2, helloworld::mean({1, 2, 4}));
}

TEST(StatsTest, MedianOfAnOddNumberOfElements) {
  EXPECT_EQ(3, helloworld::median({5, 1, 3}));
}

// EXPECTED TO FAIL: the even-sized case is not averaged.
TEST(StatsTest, MedianOfAnEvenNumberOfElements) {
  EXPECT_EQ(2, helloworld::median({1, 2, 3, 4}));
}

TEST(StatsTest, MaxValueFindsTheLargestElement) {
  EXPECT_EQ(9, helloworld::maxValue({3, 1, 4, 1, 5, 9, 2, 6}));
}

TEST(StatsTest, MaxValueHandlesNegativeNumbers) {
  EXPECT_EQ(-1, helloworld::maxValue({-7, -1, -3}));
}

}  // namespace
