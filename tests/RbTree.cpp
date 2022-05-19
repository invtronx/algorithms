#include <array>
#include <utility>
#include <algorithm>
#include <gtest/gtest.h>
#include "RbTree.hpp"

int gCount, gMaxHeight, gBlackCount;

void Traverse(const RbNode<int> *x, int height)
{
    gMaxHeight = std::max(gMaxHeight, height);
    ++gCount;
    if (x->left != RbNode<int>::NIL)
        Traverse(x->left, height + 1);
    if (x->right != RbNode<int>::NIL)
        Traverse(x->right, height + 1);
}

bool HasBoundedHeight(const RbTree<int> &r)
{
    gCount = gMaxHeight = 0;
    Traverse(r.Root(), 0);
    auto lg = [](int x) {
        return 31 - __builtin_clz(x);
    };
    return gCount < 2 || gMaxHeight <= 2 * lg(gCount - 1);
}

bool CheckRbProperty(const RbNode<int> *x, int blackCount)
{
    if (x == RbNode<int>::NIL) {
        if (gBlackCount == -1)
            gBlackCount = blackCount;
        return x->color == Color::BLACK && gBlackCount == blackCount;
    }
    if (x->left != RbNode<int>::NIL && x->key < x->left->key)
        return false;
    if (x->right != RbNode<int>::NIL && x->key > x->right->key)
        return false;
    if (x->color == Color::RED && (x->left->color != Color::BLACK ||
                x->right->color != Color::BLACK))
        return false;
    blackCount += x->color == Color::BLACK;
    return CheckRbProperty(x->left, blackCount) && CheckRbProperty(x->right, blackCount);
}

bool HasRbProperty(const RbTree<int> &r)
{
    gBlackCount = -1;
    return r.Root()->color == Color::BLACK && CheckRbProperty(r.Root(), 0) &&
        HasBoundedHeight(r);
}

TEST(RbTree, TestRbProperty)
{
    RbTree<int> tree;
    for (int i = 0; i < 100000; i++)
        tree.Insert(new RbNode<int>{rand()});
    EXPECT_TRUE(HasRbProperty(tree));
}

class RbTreeFixture : public ::testing::Test {
protected:
    void SetUp() override
    {
        // Trust the implementation of `Insert`
        for (int i = 0; i < small.size(); i++) {
            smallNodes[i] = new RbNode<int>{small[i]};
            smallT.Insert(smallNodes[i]);
        }
    }

    RbTree<int> smallT;
    std::array<int, 7> small {3, 1, 8, 10, 5, 23, 4};
    std::array<RbNode<int>*, 7> smallNodes;
};

TEST(HelperFuncs, TestLg) {
    auto lg = [](int x) { return 31 - __builtin_clz(x); };
    std::array<int, 8> in {2, 4, 8, 7, 15, 17, 256, 1023};
    std::array<int, 8> out {1, 2, 3, 2, 3, 4, 8, 9};
    for (int i = 0; i < in.size(); i++)
        ASSERT_EQ(lg(in[i]), out[i]);
}

TEST_F(RbTreeFixture, BasicProperties) {
    ASSERT_TRUE(HasRbProperty(smallT));
}

TEST_F(RbTreeFixture, MinAndMax) {
    EXPECT_EQ(smallT.Minimum()->key, 1);
    EXPECT_EQ(smallT.Maximum()->key, 23);
}

TEST_F(RbTreeFixture, Search) {
    for (int i = 0; i < small.size(); i++)
        EXPECT_EQ(smallT.Search(small[i]), smallNodes[i]);
    for (int nonVals : {0, 2, 13, 25})
        EXPECT_EQ(smallT.Search(nonVals), RbNode<int>::NIL);
}

TEST_F(RbTreeFixture, PredAndSucc) {
    std::sort(small.begin(), small.end());
    for (int i = 1; i < small.size(); i++) {
        EXPECT_EQ(smallT.Predecessor(smallT.Search(small[i]))->key,  small[i - 1]);
        EXPECT_EQ(smallT.Successor(smallT.Search(small[i - 1]))->key, small[i]);
    }
}

TEST_F(RbTreeFixture, Delete) {
    smallT.Delete(smallNodes[1]), delete smallNodes[1];
    EXPECT_EQ(smallT.Minimum(), smallNodes[0]);
    smallT.Delete(smallNodes[5]), delete smallNodes[5];
    EXPECT_EQ(smallT.Maximum(), smallNodes[3]);

    EXPECT_TRUE(HasRbProperty(smallT));
    std::sort(small.begin(), small.end());
    for (int i = 1; i + 2 < small.size(); i++) {
        EXPECT_EQ(smallT.Predecessor(smallT.Search(small[i + 1]))->key,  small[i]);
        EXPECT_EQ(smallT.Successor(smallT.Search(small[i]))->key, small[i + 1]);
    }
}

