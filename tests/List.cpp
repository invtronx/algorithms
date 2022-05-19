#include <array>
#include <gtest/gtest.h>
#include "List.hpp"

TEST(ListTest, TestInsert) {
    List<int> ls;
    std::array<int, 4> valarray {1, 2, 3, 4};
    for (auto r = valarray.rbegin(); r != valarray.rend(); r++)
        ls.InsertVal(*r);
    ListNode<int> *trav = ls.head();
    int i = 0;
    do {
        ASSERT_EQ(valarray[i], trav->data);
        trav = trav->next, ++i;
    } while (trav != List<int>::NIL);
}

TEST(ListTest, TestEmpty) {
    List<double> emptyList;
    ASSERT_TRUE(emptyList.empty());
    emptyList.InsertVal(0.12);
    ASSERT_FALSE(emptyList.empty());
}

// Create a text fixture after confirming InsertVal works correctly
class ListTestFixture : public ::testing::Test {
protected:
    void SetUp() override {
        for (auto r = valarr.rbegin(); r != valarr.rend(); r++)
            ls.InsertVal(*r);
    }

    List<int> ls;
    static constexpr int n = 4;
    std::array<int, n> valarr {1, 8, 18, 3};
};

TEST_F(ListTestFixture, TestIterator) {
    int i = 0;
    for (int curVal : ls)
        ASSERT_EQ(curVal, valarr[i++]);
    i = 0;
    for (auto it = ls.begin(); it != ls.end(); it++)
        ASSERT_EQ(*it, valarr[i++]);
}

TEST_F(ListTestFixture, TestDispose) {
    // Linked List: 1, 8, 18, 3
    ls.Dispose(ls.head());
    EXPECT_EQ(ls.head()->data, 8);
    ls.Dispose(ls.tail());
    EXPECT_EQ(ls.tail()->data, 18);
    ls.Dispose(ls.head());
    ls.Dispose(ls.tail());
    EXPECT_TRUE(ls.empty());
}

TEST_F(ListTestFixture, TestDisposeOnIteration1) {
    for (auto lx = ls.begin(); lx != ls.end(); lx++)
        if (lx != ls.begin())
            ls.Dispose(lx.node());
    EXPECT_EQ(ls.head()->data, 1);
    EXPECT_EQ(ls.size(), 1);
}

TEST_F(ListTestFixture, TestDisposeOnIteration2) {
    ListNode<int> *prv = ls.head();
    for (auto lx = ls.begin(); lx != ls.end(); lx++)
        if (lx != ls.begin())
            ls.Dispose(prv), prv = lx.node();
    EXPECT_EQ(ls.head()->data, 3);
    EXPECT_EQ(ls.size(), 1);
}

TEST_F(ListTestFixture, TestEmptyIteration) {
    List<int> emp;
    for (auto lx = emp.begin(); lx != emp.end(); lx++)
        ADD_FAILURE();
    EXPECT_TRUE(emp.empty());
}

TEST_F(ListTestFixture, TestUnion) {
    List<int> nxtls;
    std::array<int, 2> nxtarr {4, 7};
    for (auto r = nxtarr.rbegin(); r != nxtarr.rend(); r++)
        nxtls.InsertVal(*r);
    ls.Union(std::move(nxtls));  // ls = ls + nxtls

    EXPECT_EQ(ls.size(), valarr.size() + nxtarr.size());
    int i = 0;
    for (int curVal : ls) {
        int expected = i < n ? valarr[i] : nxtarr[i - n];
        EXPECT_EQ(curVal, expected);
        ++i;
    }
}

