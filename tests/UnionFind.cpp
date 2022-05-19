#include <utility>
#include <initializer_list>
#include "UnionFind.hpp"
#include <gtest/gtest.h>

class UnionFindFixture : public ::testing::Test {
protected:
    void SetUp() override
    {
        for (int x = 0; x < n; x++)
            mem[x] = MakeSet(x);
    }

    void TearDown() override
    {
        for (int x = 0; x < n; x++)
            delete mem[x];
    }

    void Link(std::initializer_list<std::pair<int, int>> pairs)
    {
        for (std::pair<int, int> p : pairs)
            Union(mem[p.first], mem[p.second]);
    }

    static constexpr int n = 10;
    Member<int>* mem[n];
};

TEST_F(UnionFindFixture, FindSet) {
    for (int x = 0; x < n; x++)
        EXPECT_EQ(FindSet(mem[x]), mem[x]);
}

TEST_F(UnionFindFixture, Union) {
    Union(mem[0], mem[1]);
    EXPECT_EQ(mem[0]->parent, mem[1]);
    EXPECT_EQ(mem[1]->parent, mem[1]);
    Union(mem[2], mem[1]);
    Union(mem[7], mem[6]);
    Union(mem[6], mem[2]);
    EXPECT_EQ(mem[6]->parent, mem[1]);
}

TEST_F(UnionFindFixture, UnionAndFindSet) {
    Link({{0, 1}, {1, 2}, {3, 1}, {5, 4}, {6, 7}, {4, 7}});
    for (int x = 0; x <= 3; x++)
        EXPECT_EQ(FindSet(mem[x]), mem[1]);
    for (int x = 4; x <= 7; x++)
        EXPECT_EQ(FindSet(mem[x]), mem[7]);
}

