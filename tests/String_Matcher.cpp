#include <string>
#include <vector>
#include <gtest/gtest.h>

#include "String_Matcher.hpp"
#include "KMP.hpp"

class StringMatcherFixture : public ::testing::Test {
protected:
    void SetUp() override
    {
        matcher = &kmp;
    }

    StringMatcher *matcher;
    KMP kmp;
};

TEST_F(StringMatcherFixture, Numerical) {
    std::string text = "02475404759";
    std::string pattern = "475";
    std::vector<int> supposed({ 2, 7 });
    std::vector<int> matches(matcher->match(text, pattern));
    EXPECT_EQ(supposed, matches);
}

TEST_F(StringMatcherFixture, Words) {
    std::string text = "Learn. Read. Create. Preach.";
    std::string pattern = "ea";
    std::vector<int> supposed({ 1, 8, 15, 23 });
    std::vector<int> matches(matcher->match(text, pattern));
    EXPECT_EQ(supposed, matches);
}

TEST_F(StringMatcherFixture, General) {
    std::string text = "bin 0x01 >> 0x02 -0x code";
    std::string pattern = "0x0";
    std::vector<int> supposed({ 4, 12 });
    std::vector<int> matches(matcher->match(text, pattern));
    EXPECT_EQ(supposed, matches);
}

