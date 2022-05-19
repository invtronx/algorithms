#ifndef KMP_hpp
#define KMP_hpp

#include "String_Matcher.hpp"

class KMP : public StringMatcher {
public:
    std::vector<int> match(const std::string &T, const std::string &P);

private:
    std::vector<int> preprocess(const std::string &P);
};

#endif  // KMP_hpp

