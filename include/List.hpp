#ifndef List_hpp
#define List_hpp

#include <iterator>

template<typename T>
struct ListNode {
    T data;
    ListNode *next;
    ListNode *prev;

    explicit ListNode(T _data = T{}) : data{_data} {}
};

template<typename T>
class List {
public:
    class iterator : public std::iterator<std::forward_iterator_tag, T> {
    public:
        iterator(List *lp, ListNode<T> *node)
        {
            this->lp = lp;
            this->cur = node, this->nxt = node->next;
        }
        iterator(const iterator &it)
        {
            this->lp = it.lp;
            this->cur = it.cur, this->nxt = it.nxt;
        }
        // LegacyForwardIterator Requirements
        iterator& operator++()
        {
            this->cur = this->nxt, this->nxt = this->nxt->next;
            return *this;
        }
        iterator operator++(int)
        {
            iterator copy(*this);
            this->cur = this->nxt, this->nxt = this->nxt->next;
            return copy;
        }
        // LegacyInputIterator Requirements
        bool operator==(const iterator &it)
        {
            return this->lp == it.lp && this->cur == it.cur;
        }
        bool operator!=(const iterator &it)
        {
            return !(*this == it);
        }
        T& operator*()
        {
            return this->cur->data;
        }
        T* operator->()
        {
            return &this->cur->data;
        }
        // Additional Access Function
        ListNode<T>* node()
        {
            return this->cur;
        }

    private:
        List *lp;
        ListNode<T> *cur, *nxt;
    };

    static ListNode<T> __NIL, *NIL;  // sentinel NIL node

    void InsertVal(T element);
    void Insert(ListNode<T> *node);

    void Delete(ListNode<T> *node);
    void Dispose(ListNode<T> *node);  // deletes and deallocates the node

    void Union(List&& ls);

    const ListNode<T>* head() const;
    ListNode<T>* head();
    const ListNode<T>* tail() const;
    ListNode<T>* tail();

    void clear();

    iterator begin();
    iterator end();

    size_t size() const;
    bool empty() const;

    ~List();

private:
    size_t size_ {0};
    ListNode<T> *head_ {NIL};
    ListNode<T> *tail_ {NIL};
};

template<typename T>
ListNode<T> List<T>::__NIL = ListNode<T>();

template<typename T>
ListNode<T>* List<T>::NIL = &__NIL;

template<typename T>
void List<T>::InsertVal(T element)
{
    ListNode<T> *node = new ListNode<T>(element);
    Insert(node);
}

template<typename T>
void List<T>::Insert(ListNode<T> *node)
{
    if (size_ == 0) {
        head_ = tail_ = node;
        node->next = node->prev = NIL;
    } else {
        node->next = head_, head_->prev = node;
        node->prev = NIL, head_ = node;
    }
    ++size_;
}

template<typename T>
void List<T>::Delete(ListNode<T> *node)
{
    node->prev->next = node->next;
    node->next->prev = node->prev;
    if (node == head_)
        head_ = node->next;
    if (node == tail_)
        tail_ = node->prev;
    --size_;
}

template<typename T>
void List<T>::Dispose(ListNode<T> *node)
{
    Delete(node);
    delete node;
}

template<typename T>
void List<T>::Union(List<T>&& ls)
{
    if (ls.size_ == 0) {
        return;
    } else if (size_ == 0) {
        size_ = ls.size_, head_ = ls.head_, tail_ = ls.tail_;
    } else {
        tail_->next = ls.head_, ls.head_->prev = tail_;
        size_ += ls.size_;
    }
    ls.clear();
}

template<typename T>
const ListNode<T>* List<T>::head() const
{
    return head_;
}

template<typename T>
ListNode<T>* List<T>::head()
{
    return head_;
}

template<typename T>
const ListNode<T>* List<T>::tail() const
{
    return tail_;
}

template<typename T>
ListNode<T>* List<T>::tail()
{
    return tail_;
}

template<typename T>
void List<T>::clear()
{
    head_ = tail_ = NIL, size_ = 0;
}

template<typename T>
typename List<T>::iterator List<T>::begin()
{
    return iterator(this, head_);
}

template<typename T>
typename List<T>::iterator List<T>::end()
{
    return iterator(this, NIL);
}

template<typename T>
size_t List<T>::size() const
{
    return size_;
}

template<typename T>
bool List<T>::empty() const
{
    return size_ == 0;
}

template<typename T>
List<T>::~List()
{
    for (auto lx = begin(); lx != end(); lx++)
        delete lx.node();
}

#endif  /* List_hpp */

