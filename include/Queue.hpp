//
//  Queue.hpp
//  DataStructures
//
//  Created by Ye Min Aung on 5/15/21.
//

#ifndef Queue_hpp
#define Queue_hpp

#include <cstddef>
#include <algorithm>
#include <exception>

template<typename T>
class Queue {
public:
    explicit Queue();   // default constructor
    Queue(const Queue &q); // copy constructor
    Queue(Queue &&q);  // move constructor
    
    Queue& operator=(const Queue &q);   // copy assignment
    Queue& operator=(Queue &&q);    // move assignment
    
    void Enqueue(T elem);
    T Dequeue();
    
    inline bool IsEmpty();
    T Front();  // access the next element to be dequeued
    T Back();   // access the last element to be dequeued
    inline int Size();
    
    ~Queue();   // destructor

private:
    T* buf;
    size_t bufSize;
    unsigned head;
    unsigned tail;
    
    static constexpr size_t ALLOCSZ { 10 }; // initial allocation size
    
    void Extend();  // extend the allocation size of queue
    inline void CopyStaticData(const Queue &q);  // copies static members
};

//
// Inline function definitions
//

template<typename T>
inline bool Queue<T>::IsEmpty()
{
    return head == tail;
}

template<typename T>
inline int Queue<T>::Size()
{
    return (tail + bufSize - head) % bufSize;
}

template<typename T>
inline void Queue<T>::CopyStaticData(const Queue<T> &q)
{
    bufSize = q.bufSize;
    head = q.head, tail = q.tail;
}

//
// Non-inline function definitions (specific to templates)
//

template<typename T>
Queue<T>::Queue()
: bufSize{ALLOCSZ}, head{0}, tail{0}
{
    buf = new T[bufSize];
}

template<typename T>
Queue<T>::Queue(const Queue &q)
: bufSize{q.bufSize}, head{0}, tail{0}
{
    buf = new T[bufSize];
    std::copy(q.buf, q.buf + bufSize, buf);
}

template<typename T>
Queue<T>::Queue(Queue &&q)
: bufSize{q.bufSize}, head{q.head}, tail{q.tail}
{
    buf = q.buf;
    q.buf = nullptr;
    q.size = q.head = q.tail = 0;
}

template<typename T>
Queue<T>& Queue<T>::operator=(const Queue &q)
{
    if (bufSize != q.bufSize) {
        delete[] buf;
        buf = new T[q.bufSize];
    }
    CopyStaticData(q);
    std::copy(q.buf, q.buf + q.bufSize, buf);
    return *this;
}

template<typename T>
Queue<T>& Queue<T>::operator=(Queue &&q)
{
    delete[] buf;
    buf = q.buf;
    CopyStaticData(q);
    q.buf = nullptr;
    q.size = q.head = q.tail = 0;
    return *this;
}

template<typename T>
void Queue<T>::Enqueue(T elem)
{
    if (head == (tail + 1) % bufSize)
        Extend();
    buf[tail] = elem;
    tail = (tail + 1) % bufSize;
}

template<typename T>
T Queue<T>::Dequeue()
{
    if (head == tail)
        throw new std::underflow_error("Cannot dequeue an empty queue.");
    T elem = buf[head];
    head = (head + 1) % bufSize;
    return elem;
}

template<typename T>
T Queue<T>::Front()
{
    if (head == tail)
        throw new std::underflow_error("Cannot inspect an empty queue.");
    return buf[head];
}

template<typename T>
T Queue<T>::Back()
{
    if (head == tail)
        throw new std::underflow_error("Cannot inspect an empty queue");
    return buf[tail - 1];
}

template<typename T>
Queue<T>::~Queue()
{
    delete[] buf;
}

//
// Private Member Functions
//

template<typename T>
void Queue<T>::Extend()
{
    T* extended = new T[bufSize * 2];
    
    int updatedTail = 0;
    for (int i = 0; i != tail; i = (i + 1) % bufSize) {
        extended[updatedTail++] = buf[i];
    }
    head = 0, tail = updatedTail;
    bufSize *= 2;
    
    delete[] buf;
    buf = extended;
}

#endif /* Queue_hpp */
