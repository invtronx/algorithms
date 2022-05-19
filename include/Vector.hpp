// 
//  Vector.hpp
//  DataStructures
//
//  Created by Ye Min Aung on 5/15/21.
//

#ifndef Vector_hpp
#define Vector_hpp

#include <initializer_list>
#include <iterator>
#include <algorithm>
#include <iostream>

// Source: https://github.com/gcc-mirror/gcc/blob/7b35a939b8cb869efb830701cef4fa1dc5ff4020/libstdc%2B%2B-v3/include/bits/stl_iterator_base_types.h#L230
template<typename _InIter>
using _RequireInputIter = typename std::enable_if<std::is_convertible<typename
    std::iterator_traits<_InIter>::iterator_category, std::input_iterator_tag>::value>::type;

template<typename T>
class Vector {
public:
    using value_type = T;
    using size_type = size_t;

    explicit Vector();
    Vector(size_t size, const T& def = T());
    Vector(std::initializer_list<T> elems);

    template<typename InputIterator, typename = _RequireInputIter<InputIterator>>
    Vector(InputIterator first, InputIterator last);

    Vector(const Vector &v);   // copy constructor
    Vector(Vector &&v);    // move constructor
    
    Vector& operator=(const Vector &v); // copy assignment
    Vector& operator=(Vector &&v);  // move assignment

    bool operator==(const Vector &v);   // value equality operator

    const T& operator[](int index) const;
    T& operator[](int index);

    const T& front() const;
    T& front();
    const T& back() const;
    T& back();

    // Check out Stanford's CS106b's implementation of Vector 
    // Source: https://cs.stanford.edu/people/eroberts/courses/cs106b/materials/cppdoc/vector-h.html
    class iterator : public std::iterator<std::random_access_iterator_tag, T> {
    public:
        iterator()
        {
            this->vp = nullptr;
        }
        iterator(const Vector *vp, int index)
        {
            this->vp = vp;
            this->index = index;
        }
        iterator(const iterator &it)
        {
            this->vp = it.vp;
            this->index = it.index;
        }
        // LegacyRandomAccessIterator Requirements
        iterator& operator+=(int n)
        {
            this->index += n;
            return *this;
        }
        iterator& operator-=(int n)
        {
            this->index -= n;
            return *this;
        }
        iterator operator+(int n)
        {
            iterator copy(*this);
            return copy += n;
        }
        iterator operator-(int n)
        {
            iterator copy(*this);
            return copy -= n;
        }
        int operator-(const iterator &rhs)
        {
            return index - rhs.index;
        }
        iterator operator[](int index)
        {
            return (*vp)[index];
        }
        bool operator<(const iterator &rhs)
        {
            return this->index < rhs.index;
        }
        bool operator<=(const iterator &rhs)
        {
            return this->index <= rhs.index;
        }
        bool operator>(const iterator &rhs)
        {
            return this->index > rhs.index;
        }
        bool operator>=(const iterator &rhs)
        {
            return this->index >= rhs.index;
        }
        // LegacyBidirectionalIterator Requirements
        iterator& operator--()
        {
            --this->index;
            return *this;
        }
        iterator operator--(int)
        {
            iterator copy(*this);
            --this->index;
            return copy;
        }
        // LegacyForwardIterator Requirements
        iterator& operator++()
        {
            ++this->index;
            return *this;
        }
        iterator operator++(int)
        {
            iterator copy(*this);
            ++this->index;
            return copy;
        }
        // LegacyInputIterator Requirements
        bool operator==(const iterator &it)
        {
            return this->vp == it.vp && this->index == it.index;
        }
        bool operator!=(const iterator &it)
        {
            return !(*this == it);
        }
        T& operator*()
        {
            return vp->arr[index];
        }
        T* operator->()
        {
            return &vp->arr[index];
        }

    private:
        const Vector *vp;
        int index;
    };

    using reverse_iterator = typename std::reverse_iterator<iterator>;

    iterator begin();
    iterator end();

    reverse_iterator rbegin();
    reverse_iterator rend();
    
    bool empty() const;
    size_t size() const;
    void reserve(size_t size);
    size_t capacity() const;
    void shrink_to_fit();
    
    void clear();
    void push_back(T elem);
    T pop_back();
    void resize(int size);
    
    ~Vector();  // destructor

private:
    T* arr;
    size_t cap;
    size_t sz;
    
    static size_t INITIAL_CAP;
    
    void expand_capacity();
};

template<typename T>
size_t Vector<T>::INITIAL_CAP { 10 };

template<typename T>
Vector<T>::Vector()
: cap{INITIAL_CAP}, sz{0}
{
    arr = new T[cap];
}

template<typename T>
Vector<T>::Vector(size_t size, const T& def)
: cap{std::max(size, INITIAL_CAP)}, sz{size}
{
    arr = new T[cap];
    std::fill(arr, arr + sz, def);
}

template<typename T>
Vector<T>::Vector(std::initializer_list<T> elems)
: cap{std::max(elems.size(), INITIAL_CAP)}, sz{elems.size()}
{
    arr = new T[cap];
    std::copy(elems.begin(), elems.end(), arr);
}

template<typename T>
template<typename InputIterator, typename>
Vector<T>::Vector(InputIterator first, InputIterator last)
: sz{0}
{
    // Optimize capacity calculation if InputIter is a RandomAccessIterator
    bool bidirectional = std::is_convertible<InputIterator, std::iterator<
        std::random_access_iterator_tag, T>>::value;
    cap = bidirectional ? last - first : INITIAL_CAP;
    arr = new T[cap];
    while (first != last) {
        if (sz == cap)
            expand_capacity();
        arr[sz++] = *first++;
    }
}

template<typename T>
Vector<T>::Vector(const Vector &v)
{
    cap = v.cap, sz = v.sz;
    arr = new T[cap];
    std::copy(v.arr, v.arr + v.sz, arr);
}

template<typename T>
Vector<T>::Vector(Vector &&v)
{
    cap = v.cap, sz = v.sz;
    arr = v.arr;
    v.arr = nullptr;
    v.sz = 0;
}

template<typename T>
Vector<T>& Vector<T>::operator=(const Vector &v)
{
    if (this == &v) // handle self-assignment
        return *this;
    if (cap != v.cap) {
        delete[] arr;
        arr = new T[v.cap];
    }
    cap = v.cap, sz = v.sz;
    std::copy(v.arr, v.arr + v.sz, arr);
    return *this;
}

template<typename T>
Vector<T>& Vector<T>::operator=(Vector &&v)
{
    if (this == &v) // handle self-assignment
        return *this;
    cap = v.cap, sz = v.sz;
    delete[] arr;
    arr = v.arr;
    v.arr = nullptr;
    v.sz = 0;
    return *this;
}

template<typename T>
bool Vector<T>::operator==(const Vector &v)
{
    return std::equal(arr, arr + sz, v.arr) && sz == v.sz && cap == v.cap;
}

template<typename T>
const T& Vector<T>::operator[](int index) const
{
    return arr[index];  // doesn't check for array bounds
}

template<typename T>
T& Vector<T>::operator[](int index)
{
    return arr[index];
}

template<typename T>
const T& Vector<T>::front() const
{
    return arr[0];
}

template<typename T>
T& Vector<T>::front()
{
    return arr[0];
}

template<typename T>
const T& Vector<T>::back() const
{
    return arr[sz - 1];
}

template<typename T>
T& Vector<T>::back()
{
    return arr[sz - 1];
}

template<typename T>
typename Vector<T>::iterator Vector<T>::begin()
{
    return iterator(this, 0);
}

template<typename T>
typename Vector<T>::iterator Vector<T>::end()
{
    return iterator(this, sz);
}

template<typename T>
typename Vector<T>::reverse_iterator Vector<T>::rbegin()
{
    return reverse_iterator(iterator(this, sz));
}

template<typename T>
typename Vector<T>::reverse_iterator Vector<T>::rend()
{
    return reverse_iterator(iterator(this, 0));
}

template<typename T>
bool Vector<T>::empty() const
{
    return sz == 0;
}

template<typename T>
size_t Vector<T>::size() const
{
    return sz;
}

template<typename T>
void Vector<T>::reserve(size_t size)
{
    if (size <= cap) return;
    cap = size;
    T* arr_n = new T[cap];
    std::move(arr, arr + sz, arr_n);
    delete[] arr;
    arr = arr_n;
}

template<typename T>
size_t Vector<T>::capacity() const
{
    return cap;
}

template<typename T>
void Vector<T>::shrink_to_fit()
{
    if (cap == sz) return;
    cap = sz;
    T* arr_n = new T[cap];
    std::move(arr, arr + sz, arr_n);
    delete[] arr;
    arr = arr_n;
}

template<typename T>
void Vector<T>::clear()
{
    // TODO: Learn more about template parameters especially on pointer types and memory 
    // allocation
    sz = 0;
    // Free the memory of the old elements ... (necessary?)
}

template<typename T>
void Vector<T>::push_back(T elem)
{
    if (sz == cap)
        expand_capacity();
    arr[sz++] = elem;
}

template<typename T>
T Vector<T>::pop_back()
{
    if (sz == 0)
        throw new std::out_of_range("Cannot pop back an empty vector.");
    return arr[--sz];
}

template<typename T>
void Vector<T>::resize(int size)
{
    if (size > cap) // expand capacity if necessary
        reserve(size);
    sz = size;
}

template<typename T>
Vector<T>::~Vector()
{
    delete[] arr;
}

//
// Private member functions
//

template<typename T>
void Vector<T>::expand_capacity()
{
    cap *= 2;
    T* arr_n = new T[cap];
    std::move(arr, arr + sz, arr_n);
    delete[] arr;
    arr = arr_n;
}

#endif /* Vector_hpp */
