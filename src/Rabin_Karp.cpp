#include <cmath>
#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>

#include "Rabin_Karp.hpp"

int ModExp(int base, int exp, int mod)
{
    if (exp == 0)
        return 1;
    if (exp == 1)
        return base;

    int x = ModExp(base, exp / 2, mod);
    int y = (x * x) % mod;
    if (exp % 2)
        y = (y * base) % mod;
    return y;
}

std::vector<int> RabinKarp::_RabinKarp(const std::string &T, const std::string &P,
        std::unordered_map<char, int> M, int D, int Q)
{
    const int n = T.length();
    const int m = P.length();
    const int h = ModExp(D, m - 1, Q);

    int p = 0, t = 0;
    for (int i = 0; i < m; i++) {
        p = (D * p + M[P[i]]) % Q;
        t = (D * t + M[T[i]]) % Q;
    }

    std::vector<int> matches;
    for (int s = 0; s < n - m + 1; s++) {
        if (p == t && P == T.substr(s, m))
            matches.push_back(s);
        if (s < n - m)
            t = (D * (t - (M[T[s]] * h) % Q) + M[T[s + m]]) % Q;
    }
    return matches;
}

std::vector<int> RabinKarp::match(const std::string &T, const std::string &P)
{
    const int Q = 1000007;
    int D = 0;
    std::unordered_map<char, int> M;
    for (char ch : (T + P))
        if (M.count(ch) == 0)
            M[ch] = D++;
    return _RabinKarp(T, P, M, D, Q);
}

