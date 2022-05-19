//
//  Priority_Queue.hpp
//  DataStructures
//
//  Created by Ye Min Aung on 5/15/21.
//

#ifndef Priority_Queue_hpp
#define Priority_Queue_hpp

#include <functional>
#include <exception>
#include "Vector.hpp"

template<typename T, typename C = std::less<T>>
class Priority_Queue {
    // NOTE: using std::less<T> as C yields a max heap (not a min heap)
public:
    explicit Priority_Queue(const C &c = C());
    Priority_Queue(const Vector<T> &v, const C &c = C());

    template<typename InputIterator>
    Priority_Queue(InputIterator first, InputIterator last, const C &c = C());

    Priority_Queue(const Priority_Queue &q);    // copy constructor
    Priority_Queue(Priority_Queue &&q); // move constructor

    Priority_Queue& operator=(const Priority_Queue &q);
    Priority_Queue& operator=(Priority_Queue &&q);

    void enqueue(T elem);
    T dequeue();

    const T& front() const;
    T& front();

    bool empty() const;
    size_t size() const;
    const Vector<T>& data() const;

    ~Priority_Queue() = default;

private:
    Vector<T> cont;
    C comp;

    static inline int parent(int idx);
    static inline int left(int idx);
    static inline int right(int idx);

    static void heapify(Priority_Queue &q, int idx);
    static void make_heap(Priority_Queue &q);
};

template<typename T, typename C>
Priority_Queue<T, C>::Priority_Queue(const C &c)
{
    cont = Vector<T>();
    comp = c;
}

template<typename T, typename C>
Priority_Queue<T, C>::Priority_Queue(const Vector<T> &v, const C &c)
: cont(v), comp(c)
{
    make_heap(*this);
}

template<typename T, typename C>
template<typename InputIterator>
Priority_Queue<T, C>::Priority_Queue(InputIterator first, InputIterator last, const C &c)
: cont(first, last), comp(c)
{
    make_heap(*this);
}

template<typename T, typename C>
Priority_Queue<T, C>::Priority_Queue(const Priority_Queue<T, C> &q)
{
    cont = q.cont;
    comp = q.comp;
}

template<typename T, typename C>
Priority_Queue<T, C>::Priority_Queue(Priority_Queue<T, C> &&q)
{
    cont = std::move(q.cont);
    comp = q.comp;
}

template<typename T, typename C>
Priority_Queue<T, C>& Priority_Queue<T, C>::operator=(const Priority_Queue<T, C> &q)
{
    if (this == &q)
        return *this;
    cont = q.cont;
    comp = q.comp;
    return *this;
}

template<typename T, typename C>
Priority_Queue<T, C>& Priority_Queue<T, C>::operator=(Priority_Queue<T, C> &&q)
{
    if (this == &q)
        return *this;
    cont = std::move(q.cont);
    comp = q.comp;
    return *this;
}

template<typename T, typename C>
void Priority_Queue<T, C>::enqueue(T elem)
{
    cont.push_back(elem);
    int idx = cont.size() - 1;
    while (idx > 0 && comp(cont[parent(idx)], elem)) {
        cont[idx] = cont[parent(idx)];
        idx = parent(idx);
    }
    cont[idx] = elem;
}

template<typename T, typename C>
T Priority_Queue<T, C>::dequeue()
{
    if (cont.size() == 0)
        throw new std::out_of_range("Cannot dequeue an empty priority queue.");

    if (cont.size() == 1)
        return cont.pop_back();

    T elem = cont.front();
    std::swap(cont.front(), cont.back());
    cont.pop_back();
    heapify(*this, 0);
    return elem;
}

template<typename T, typename C>
const T& Priority_Queue<T, C>::front() const
{
    return cont.front();
}

template<typename T, typename C>
T& Priority_Queue<T, C>::front()
{
    return cont.front();
}

template<typename T, typename C>
bool Priority_Queue<T, C>::empty() const
{
    return cont.size() == 0;
}

template<typename T, typename C>
size_t Priority_Queue<T, C>::size() const
{
    return cont.size();
}

template<typename T, typename C>
const Vector<T>& Priority_Queue<T, C>::data() const
{
    return cont;
}

template<typename T, typename C>
inline int Priority_Queue<T, C>::parent(int idx)
{
    return ((idx + 1) / 2) - 1;
}

template<typename T, typename C>
inline int Priority_Queue<T, C>::left(int idx)
{
    return 2 * idx + 1;
}

template<typename T, typename C>
inline int Priority_Queue<T, C>::right(int idx)
{
    return 2 * idx + 2;
}

template<typename T, typename C>
void Priority_Queue<T, C>::heapify(Priority_Queue<T, C> &q, int idx)
{
    int pidx = idx, sz = q.cont.size();
    while (idx < sz) {
        if (left(idx) < sz && q.comp(q.cont[pidx], q.cont[left(idx)]))
            pidx = left(idx);
        if (right(idx) < sz && q.comp(q.cont[pidx], q.cont[right(idx)]))
            pidx = right(idx);
        if (pidx == idx) break;
        std::swap(q.cont[pidx], q.cont[idx]);
        idx = pidx;
    }
}

template<typename T, typename C>
void Priority_Queue<T, C>::make_heap(Priority_Queue<T, C> &q)
{
    int start = q.cont.size() / 2 - 1;
    for (int idx = start; idx >= 0; --idx)
        heapify(q, idx);
}

#endif /* Priority_Queue_hpp */
