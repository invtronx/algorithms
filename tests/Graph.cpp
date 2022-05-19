#include <vector>
#include <gtest/gtest.h>
#include "Graph.hpp"

TEST(PrimAlgorithm, SimpleGraph) {
    Graph simple(3);
    simple.AddEdge(0, 2, 2);
    simple.AddEdge(0, 1, 3);
    simple.AddEdge(1, 2, 1);

    std::vector<int> parent (simple.PrimAlgorithm());
    ASSERT_EQ(parent.size(), 3);
    EXPECT_EQ(parent, std::vector<int>({0, 2, 0}));
}

TEST(PrimAlgorithm, NormalGraphA) {
    Graph normal(5);
    std::tuple<int, int, int> edges[] = {{0, 4, 1}, {4, 1, 2}, {1, 2, 3}, {2, 3, 2},
        {3, 0, 3}, {0, 1, 4}, {1, 3, 1}};
    for (auto tup : edges) {
        int src, dest, weight;
        std::tie(src, dest, weight) = tup;
        normal.AddEdge(src, dest, weight);
    }
    std::vector<int> parent (normal.PrimAlgorithm());
    std::vector<int> expected_parent ({0, 4, 3, 1, 0});
    ASSERT_EQ(parent.size(), 5);
    EXPECT_EQ(parent, expected_parent);
}

TEST(PrimAlgorithm, NormalGraphB) {
    Graph normal(7);
    std::tuple<int, int, int> edges[] = {{0, 3, 2}, {0, 2, 4}, {0, 1, 1}, {0, 5, 3},
        {4, 3, 2}, {3, 2, 1}, {2, 1, 3}, {6, 1, 3}, {6, 5, 2}, {1, 5, 1}};
    for (auto tup : edges) {
        int src, dest, weight;
        std::tie(src, dest, weight) = tup;
        normal.AddEdge(src, dest, weight);
    }
    std::vector<int> parent (normal.PrimAlgorithm());
    std::vector<int> expected_parent ({0, 0, 3, 0, 3, 1, 5});
    ASSERT_EQ(parent.size(), 7);
    EXPECT_EQ(parent, expected_parent);
}

