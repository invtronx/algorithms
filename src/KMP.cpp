#include "KMP.hpp"
#include <vector>

std::vector<int> KMP::preprocess(const std::string &P)
{
    const int m = P.length();
    std::vector<int> pi(m + 1, 0);
    int k = 0;
    for (int q = 2; q <= m; q++) {
        while (k > 0 && P[k] != P[q - 1])
            k = pi[k];
        k += P[k] == P[q - 1];
        pi[q] = k;
    }
    return pi;
}

std::vector<int> KMP::match(const std::string &T, const std::string &P)
{
    const int n = T.length(), m = P.length();
    std::vector<int> pi(preprocess(P));
    std::vector<int> matches;
    int q = 0;
    for (int i = 1; i <= n; i++) {
        while (q > 0 && P[q] != T[i - 1])
            q = pi[q];
        q += P[q] == T[i - 1];
        if (q == m) {
            matches.push_back(i - m);
            q = pi[q];
        }
    }
    return matches;
}

