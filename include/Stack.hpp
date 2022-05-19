//
//  Stack.hpp
//  DataStructures
//
//  Created by Ye Min Aung on 5/15/21.
//

#ifndef Stack_hpp
#define Stack_hpp

#include <cstddef>
#include <algorithm>
#include <exception>

template<typename T>
class Stack {
public:
    explicit Stack();   // default constructor
    Stack(const Stack &s); // copy constructor
    Stack(Stack &&s);  // move constructor
    
    Stack& operator=(const Stack &s);   // copy assignment
    Stack& operator=(Stack &&s);    // move assignment
    
    void Push(T elem);
    T Pop();
    
    const T& Top() const;    // access the value of the next element to be popped
    T& Top();   // access the reference to the next element to be popped
    
    inline bool IsEmpty() const;
    inline const int Size() const;
    
    ~Stack();   // destructor
    
private:
    T* buf;
    size_t bufsz;
    size_t size;
    
    static constexpr size_t ALLOCSZ { 10 };
    
    void Extend();  // expand the stack allocation size
};

template<typename T>
Stack<T>::Stack()
: bufsz{ALLOCSZ}, size{0}
{
    buf = new T[bufsz];
}

template<typename T>
Stack<T>::Stack(const Stack &s)
{
    *this = s;  // call the copy assignment
}

template<typename T>
Stack<T>::Stack(Stack &&s)
{
    // TODO: Check if the move assignment is invoked here
    *this = s;  // call the move assignment
}

template<typename T>
Stack<T>& Stack<T>::operator=(const Stack &s)
{
    if (bufsz != s.bufsz) {
        delete[] buf;
        bufsz = s.bufsz;
        buf = new T[bufsz];
    }
    size = s.size;
    // works safely even when bufsz == s.bufsz and s.size > size as the extra memory will be overloaded anyways
    std::copy(buf, buf + bufsz, s.buf);
}

template<typename T>
Stack<T>& Stack<T>::operator=(Stack &&s)
{
    bufsz = s.bufsz, size = s.size;
    buf = s.buf;
    s.buf = nullptr;
    s.size = 0;
}

template<typename T>
void Stack<T>::Push(T elem)
{
    if (size == bufsz)
        Extend();
    buf[size++] = elem;
}

template<typename T>
T Stack<T>::Pop()
{
    if (size == 0)
        throw new std::underflow_error("Cannot pop an empty stack.");
    return buf[--size];
}

template<typename T>
const T& Stack<T>::Top() const
{
    if (size == 0)
        throw new std::underflow_error("Cannot access the top element of an empty stack.");
    return buf[size - 1];
}

template<typename T>
T& Stack<T>::Top()
{
    if (size == 0)
        throw new std::underflow_error("Cannot access the top element of an empty stack.");
    return buf[size - 1];
}

template<typename T>
inline bool Stack<T>::IsEmpty() const
{
    return size == 0;
}

template<typename T>
inline const int Stack<T>::Size() const
{
    return size;
}

template<typename T>
void Stack<T>::Extend()
{
    T* nbuf = new T[bufsz * 2];
    std::copy(buf, buf + bufsz, nbuf);
    bufsz *= 2;
    delete buf;
    buf = nbuf;
}

template<typename T>
Stack<T>::~Stack()
{
    delete[] buf;
}

#endif /* Stack_hpp */
