//
//  Priority_Queue.cpp
//  DataStructures
//
//  Created by Ye Min Aung on 5/15/21.
//

#include <functional>
#include <cstddef>
#include "Priority_Queue.hpp"

template<typename T>
std::ostream& operator<<(std::ostream& os, const Vector<T> &v)
{
    os << '{';
    // Don't change "i + 1 < v.size()" to "i < v.size() - 1" due to wrap-around behavior
    // of unsigned integers (e.g. when v.size() == 0, v.size() = -1 (= 1<< 32 - 1)
    for (int i = 0; i + 1 < v.size(); i++)
        os << v[i] << ", ";
    if (v.size() > 0)
        os << v[v.size() - 1];
    os << '}';
    return os;
}

int Test_Priority_Queue()
{
    Priority_Queue<int> qdef;  // should invoke the default constructor

    Priority_Queue<int, std::greater<int>> qcomp (std::greater<int>{}); // redundant
    // NOTE: Initialization of functors with taking no constructor arguments can only be
    // achieved with uniform initialization

    Vector<uint64_t> vrand (10);   // construct a vector of random numbers
    std::generate(vrand.begin(), vrand.end(), rand);
    Priority_Queue<uint64_t, std::greater<uint64_t>> rand (vrand);
    Priority_Queue<uint64_t> rrand (vrand.begin(), vrand.end());

    std::cout << "Vector<T> vrand = " << vrand << '\n';
    std::cout << "Priority_Queue<T> rand = " << rand.data() << '\n';
    std::cout << "Priority_Queue<T> rrand = " << rrand.data() << '\n';
    
    Priority_Queue<uint64_t, std::greater<uint64_t>> cpy (rand); // copy constructor
    Priority_Queue<uint64_t> moved (std::move(rrand));   // move constructor

    for (int n: { 9, 4, 1, -8, -2, 0 }) {
        qcomp.enqueue(n);  // test the enqueue function
        std::cout << "After enqueuing " << n <<  ", qcomp = " << qcomp.data() << '\n';
    }

    bool isEmpty = qdef.empty();    // test the empty function
    std::cout << "default priority queue is empty: " << isEmpty << '\n';

    int front = qcomp.front();  // test the front function
    std::cout << "front of qcomp =  " << front << '\n';

    while (!qcomp.empty()) {
        int dequeued = qcomp.dequeue();   // test the dequeue function
        std::cout << "After dequeuing " << dequeued << ", qcomp = " << qcomp.data() << '\n';
    }
    
    return 1;
}

int main()
{
    Test_Priority_Queue();
    return 0;
}

