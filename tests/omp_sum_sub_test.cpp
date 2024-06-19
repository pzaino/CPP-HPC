#include <gtest/gtest.h>
#include <vector>
#include "omp_sum_sub.hpp"

// Example test for sum and sub functions
TEST(SumSubTest, HandlesEmptyVector) {
    std::vector<int> vec;
    EXPECT_NO_THROW(parallelSum(vec));
    EXPECT_NO_THROW(parallelSub(vec));
}

TEST(SumSubTest, HandlesSingleElementVector) {
    std::vector<int> vec = {1};
    EXPECT_EQ(parallelSum(vec), 1);
    EXPECT_EQ(parallelSub(vec), -1);
}

TEST(SumSubTest, HandlesMultipleElements) {
    std::vector<int> vec = {3, 1, 2};
    EXPECT_EQ(parallelSum(vec), 6);
    EXPECT_EQ(parallelSub(vec), -6);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
