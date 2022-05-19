//
//  Vector.cpp
//  DataStructures
//
//  Created by Ye Min Aung on 5/16/21.
//

#include <iostream>
#include "Vector.hpp"

template<typename T>
std::ostream& operator<<(std::ostream& os, const Vector<T> &v)
{
    os << '{';
    for (int i = 0; i < v.size() - 1; i++)
        os << v[i] << ", ";
    if (v.size() > 0)
        os << v[v.size() - 1];
    os << '}';
    return os;
}

int TestVector()
{
    Vector<int> def_vec;  // should call the default constructor
    Vector<double> size_vec(10);    // should call the size constructor
    Vector<short> fill_vec(10, 3);  // should call the fill constructor
    Vector<int> ini_vec {5, 7, 2, 4};   // should call the initializer list constructor

    std::cout << "fill_vec = " << fill_vec << '\n';

    Vector<int> range_vec(ini_vec.begin(), ini_vec.end()); 
    Vector<int> rrange_vec(ini_vec.rbegin(), ini_vec.rend());

    Vector<int> ini_copy(ini_vec);  // should invoke the copy constructor
    Vector<short> fill_moved(std::move(fill_vec));  // should invoke move constructor

    std::cout << "range_vec = " << range_vec << '\n';
    std::cout << "rrange_vec = " << rrange_vec << '\n';

    for (double &d: size_vec)   // this should work (begin, end)
        d = (rand() % 100) / 50.0;

    std::cout << "size_vec = ";
    for (const double &cd: size_vec) // should invoke .cbegin(), .cend()
        std::cout << cd << ' ';
    std::cout << '\n';

    using Vec_rIter = typename decltype(size_vec)::reverse_iterator;
    std::cout << "reversed size_vec = ";
    // should invoke the non-const reverse iterators .rbegin(), .rend()
    for (Vec_rIter it = size_vec.rbegin(); it != size_vec.rend(); ++it)
        std::cout << *it << ' ';
    std::cout << '\n';

    Vector<double> size_copy;
    size_copy = size_vec;   // should invoke copy assignment
    auto fn = []() {
        return Vector<double>(5, 1.12);
    };
    Vector<double> d_move = fn();   // should invoke move assignment

    bool val_equal = ini_copy == ini_vec;   // should be value-equal
    std::cout << "should be value-equal = " << val_equal << '\n';
    bool self_equal = ini_copy == ini_copy; // should be self-equal
    std::cout << "should be self-equal = " << self_equal << '\n';

    std::cout << "size_vec = " << size_vec << '\n'; // should call template fn

    double d = size_vec[2];     // should invoke const subscript operator
    std::cout << "size_vec[2] = " << d << '\n';
    size_vec[1] = 0.12; // should invoke reference subscript operator
    std::cout << "size_vec (updated) = " << size_vec << '\n';

    bool empty = def_vec.empty();
    std::cout << "should be empty = " << empty << '\n';

    for (int x: { 2, 1, 7, 8, 5 })
        def_vec.push_back(x);
    std::cout << "def_vec = " << def_vec << '\n';

    int popped = def_vec.pop_back();
    std::cout << "popped back value = " << popped << '\n';
    std::cout << "def_vec (updated) = " << def_vec << '\n';

    return 1;
}

int main()
{
    TestVector();

    return 0;
}
