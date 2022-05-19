#ifndef Rabin_Karp_hpp
#define Rabin_Karp_hpp

#include <vector>
#include <string>
#include <unordered_map>

#include "String_Matcher.hpp"

class RabinKarp : public StringMatcher {
public:
    std::vector<int> match(const std::string &T, const std::string &P);

private:
    static std::vector<int> _RabinKarp(const std::string &T, const std::string &P,
            std::unordered_map<char, int> M, int D, int Q);
};

#endif  // Rabin_Karp_hpp

