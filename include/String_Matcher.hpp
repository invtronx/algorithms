#ifndef String_Matcher_hpp
#define String_Matcher_hpp

#include <string>
#include <vector>

class StringMatcher {
public:
    virtual std::vector<int> match(const std::string &T, const std::string &P)
        = 0;
};

#endif  // String_Matcher_hpp

