#include <gtest/gtest.h>
#include <vector>
#include "omp_dijkstra.hpp"

// Example test for dijkstra function
TEST(DijkstraTest, HandlesSingleNode) {
    std::vector<std::vector<int>> graph = {{0}};
    EXPECT_NO_THROW({
        auto dist = dijkstra(graph, 0);
        EXPECT_EQ(dist.size(), 1);
        EXPECT_EQ(dist[0], 0);
    });
}

TEST(DijkstraTest, HandlesMultipleNodes) {
    std::vector<std::vector<int>> graph = {
        {0, 1, 4},
        {1, 0, 2},
        {4, 2, 0}
    };
    EXPECT_NO_THROW({
        auto dist = dijkstra(graph, 0);
        EXPECT_EQ(dist.size(), 3);
        EXPECT_EQ(dist[0], 0);
        EXPECT_EQ(dist[1], 1);
        EXPECT_EQ(dist[2], 3);
    });
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
