#include <gtest/gtest.h>
#include <vector>
#include "omp_k_means_clustering.hpp"

TEST(KMeansTest, HandlesEmptyData) {
    std::vector<std::vector<float>> data;
    std::vector<int> labels = kmeans(data, 3, 10);
    EXPECT_TRUE(labels.empty());
}

TEST(KMeansTest, HandlesSingleCluster) {
    std::vector<std::vector<float>> data = {
        {1.0, 2.0},
        {1.1, 2.1},
        {0.9, 1.9}
    };
    std::vector<int> labels = kmeans(data, 1, 10);
    EXPECT_EQ(labels.size(), data.size());
    for (int label : labels) {
        EXPECT_EQ(label, 0);
    }
}

TEST(KMeansTest, HandlesMultipleClusters) {
    std::vector<std::vector<float>> data = {
        {1.0, 2.0},
        {1.1, 2.1},
        {0.9, 1.9},
        {10.0, 10.0},
        {10.1, 10.1},
        {9.9, 9.9}
    };
    std::vector<int> labels = kmeans(data, 2, 10);
    EXPECT_EQ(labels.size(), data.size());
    int count0 = std::count(labels.begin(), labels.end(), 0);
    int count1 = std::count(labels.begin(), labels.end(), 1);
    EXPECT_EQ(count0 + count1, data.size());
    EXPECT_GT(count0, 0);
    EXPECT_GT(count1, 0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
