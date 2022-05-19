#include <algorithm>
#include <array>
#include <gtest/gtest.h>
#include "FibHeap.hpp"
#include "List.hpp"

TEST(FibTest, TestInsert) {
    FibHeap h;
    std::array<int, 3> vals {4, 1, 3};
    for (auto r = vals.rbegin(); r != vals.rend(); r++)
        h.InsertVal(*r);
    // manually inspect the rootList of the fibonacci heap
    List<FibNode*> &rootl = h.rootList();
    int ival = 0;
    for (FibNode *node : rootl)
        EXPECT_EQ(node->key, vals[ival++]);
    EXPECT_EQ(h.size(), 3);
}

class FibHeapFixture : public ::testing::Test {
protected:
    void SetUp() override {
        for (int i = 0; i < medSz; i++)
            medNodes[i] = new FibNode(medLs[i]);
        for (int i = 0; i < smallSz; i++)
            smallNodes[i] = new FibNode(smallLs[i]);
        for (auto r = medNodes.rbegin(); r != medNodes.rend(); r++)
            medH.Insert(*r);
        for (auto r = smallNodes.rbegin(); r != smallNodes.rend(); r++)
            smallH.Insert(*r);
    }

    FibHeap medH, smallH;
    static constexpr int medSz = 10, smallSz = 4;
    // stores the values of the heap
    std::array<int, medSz> medLs {18, 7, 10, 2, 43, 8, 23, 3, 30, 11};
    std::array<int, smallSz> smallLs {31, 1, 9, 5};
    // stores the corresponding  fibonacci nodes of the heap
    std::array<FibNode*, medSz> medNodes;
    std::array<FibNode*, smallSz> smallNodes;
};

TEST_F(FibHeapFixture, TestExtractMin) {
    std::sort(medLs.begin(), medLs.end()); // 2, 3, 7, 8, 10, 11, 18, 23, 30, 43
    std::sort(smallLs.begin(), smallLs.end()); // 1, 5, 9, 31
    for (int medVal : medLs) {
        int medExtMin = medH.ExtractMinVal();
        EXPECT_EQ(medExtMin, medVal);
    }
    EXPECT_TRUE(medH.empty());
    for (int smallVal : smallLs) {
        int smallExtMin = smallH.ExtractMinVal();
        EXPECT_EQ(smallExtMin, smallVal);
    }
    EXPECT_TRUE(smallH.empty());
}

TEST_F(FibHeapFixture, TestDecreaseKey) {
    int smallx = *std::min_element(smallLs.begin(), smallLs.end()) - 1;
    smallH.DecreaseKey(smallNodes[2], smallx);  // decrease a key to the smallest
    EXPECT_EQ(smallH.minVal(), smallx);

    int medMinOrig = medH.minVal();
    int medx = *std::min_element(medLs.begin(), medLs.end()) + 1;
    medH.DecreaseKey(medNodes[4], medx);  // decrease a key to the 2nd smallest
    EXPECT_EQ(medMinOrig, medH.minVal());
}

TEST_F(FibHeapFixture, TestUnion) {
    int medMin = *std::min_element(medLs.begin(), medLs.end());
    EXPECT_EQ(medH.minVal(), medMin);

    medH.Union(std::move(smallH));  // union the two heaps
    int smallMin = *std::min_element(smallLs.begin(), smallLs.end());
    int allMin = std::min(medMin, smallMin);
    EXPECT_EQ(medH.minVal(), allMin);
}

