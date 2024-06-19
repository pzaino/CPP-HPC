#include <gtest/gtest.h>
#include <vector>
#include <functional>
#include "omp_mergesort.hpp"

// Example comparator function for mergeSort
bool compare(int a, int b) {
    return a < b;
}

// Example test for mergesort function
TEST(MergeSortTest, HandlesEmptyVector) {
    std::vector<int> vec;
    EXPECT_NO_THROW(mergeSort(vec, 0, vec.size() - 1, std::function<bool(int, int)>(compare)));
}

TEST(MergeSortTest, HandlesSingleElementVector) {
    std::vector<int> vec = {1};
    mergeSort(vec, 0, vec.size() - 1, std::function<bool(int, int)>(compare));
    EXPECT_EQ(vec[0], 1);
}

TEST(MergeSortTest, HandlesMultipleElements) {
    std::vector<int> vec = {3, 1, 2};
    mergeSort(vec, 0, vec.size() - 1, std::function<bool(int, int)>(compare));
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[1], 2);
    EXPECT_EQ(vec[2], 3);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
