#include <gtest/gtest.h>
#include <vector>
#include <functional>

#include "quicksort.hpp"

// Example comparator function for quicksort
bool compare(int a, int b) {
    return a < b;
}

// Example test for quicksort function
TEST(QuickSortTest, HandlesEmptyVector) {
    std::vector<int> vec;
    EXPECT_NO_THROW(quickSort(vec, 0, vec.size() - 1, std::function<bool(int, int)>(compare)));
}

TEST(QuickSortTest, HandlesSingleElementVector) {
    std::vector<int> vec = {1};
    quickSort(vec, 0, vec.size() - 1, std::function<bool(int, int)>(compare));
    EXPECT_EQ(vec[0], 1);
}

TEST(QuickSortTest, HandlesMultipleElements) {
    std::vector<int> vec = {3, 1, 2};
    quickSort(vec, 0, vec.size() - 1, std::function<bool(int, int)>(compare));
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[1], 2);
    EXPECT_EQ(vec[2], 3);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
