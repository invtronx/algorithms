#ifndef RbTree_hpp
#define RbTree_hpp

#include <functional>  // less
#include <cstdint>  // uint8_t

enum struct Color : std::uint8_t {
    RED, BLACK
};

template<typename T>
struct RbNode {
    T key;
    Color color;
    RbNode *p;
    RbNode *left;
    RbNode *right;

    static RbNode _NIL_NODE, *NIL;
};

template<typename T, typename Compare = std::less<T>>
class RbTree {
public:
    using value_type = T;
    
    void LeftRotate(RbNode<T> *x)
    {
        RbNode<T> *y = x->right;
        x->right = y->left;
        if (y->left != RbNode<T>::NIL)
            y->left->p = x;
        y->p = x->p;
        if (x->p == RbNode<T>::NIL)
            this->root = y;
        else if (x == x->p->left)
            x->p->left = y;
        else
            x->p->right = y;
        y->left = x;
        x->p = y;
    }
    
    void RightRotate(RbNode<T> *x)
    {
        RbNode<T> *y = x->left;
        x->left = y->right;
        if (y->right != RbNode<T>::NIL)
            y->right->p = x;
        y->p = x->p;
        if (x->p == RbNode<T>::NIL)
            this->root = y;
        else if (x == x->p->left)
            x->p->left = y;
        else
            x->p->right = y;
        y->right = x;
        x->p = y;
    }

    void Insert(RbNode<T> *z)
    {
        RbNode<T> *y = RbNode<T>::NIL;
        RbNode<T> *x = this->root;
        while (x != RbNode<T>::NIL) {
            y = x;
            x = comp(z->key, x->key) ? x->left : x->right;
        }
        z->p = y;
        if (y == RbNode<T>::NIL)
            this->root = z;
        else if (comp(z->key, y->key))
            y->left = z;
        else
            y->right = z;
        z->left = z->right = RbNode<T>::NIL;
        z->color = Color::RED;
        InsertFixUp(z);
    }
    
    void InsertFixUp(RbNode<T> *z)
    {
        RbNode<T> *y;
        while (z->p->color == Color::RED) {
            if (z->p == z->p->p->left) {
                y = z->p->p->right;
                if (y->color == Color::RED) {
                    z->p->color = Color:: BLACK;
                    y->color = Color::BLACK;
                    z->p->p->color = Color::RED;
                    z = z->p->p;
                } else {
                    if (z == z->p->right) {
                        z = z->p;
                        LeftRotate(z);
                    }
                    z->p->color = Color::BLACK;
                    z->p->p->color = Color::RED;
                    RightRotate(z->p->p);
                }
            } else {
                y = z->p->p->left;
                if (y->color == Color::RED) {
                    z->p->color = Color:: BLACK;
                    y->color = Color::BLACK;
                    z->p->p->color = Color::RED;
                    z = z->p->p;
                } else {
                    if (z == z->p->left) {
                        z = z->p;
                        RightRotate(z);
                    }
                    z->p->color = Color::BLACK;
                    z->p->p->color = Color::RED;
                    LeftRotate(z->p->p);
                }
            }
        }
        this->root->color = Color::BLACK;
    }
    
    void Transplant(RbNode<T> *u, RbNode<T> *v)
    {
        if (u->p == RbNode<T>::NIL)
            this->root = v;
        else if (u == u->p->left)
            u->p->left = v;
        else
            u->p->right = v;
        v->p = u->p;
    }
    
    void Delete(RbNode<T> *z)
    {
        RbNode<T> *x;
        RbNode<T> *y = z;
        Color yOriginalColor = y->color;
        if (z->left == RbNode<T>::NIL) {
            x = z->right;
            Transplant(z, z->right);
        } else if (z->right == RbNode<T>::NIL) {
            x = z->left;
            Transplant(z, z->left);
        } else {
            y = Minimum(z->right);
            yOriginalColor = y->color;
            x = y->right;
            if (y->p == z) {
                x->p = y;
            } else {
                Transplant(y, y->right);
                y->right = z->right;
                y->right->p = y;
            }
            Transplant(z, y);
            y->left = z->left;
            y->left->p = y;
            y->color = z->color;
        }
        if (yOriginalColor == Color::BLACK)
            DeleteFixUp(x);
    }
    
    void DeleteFixUp(RbNode<T> *x)
    {
        RbNode<T> *w;
        while (x != this->root && x->color == Color::BLACK) {
            if (x == x->p->left) {
                w = x->p->right;
                if (w->color == Color::RED) {
                    w->color = Color::BLACK;
                    x->p->color = Color::RED;
                    LeftRotate(x->p);
                    w = x->p->right;
                }
                if (w->left->color == Color::BLACK && w->right->color == Color::BLACK) {
                    w->color = Color::RED;
                    x = x->p;
                } else {
                    if (w->right->color == Color::BLACK) {
                        w->left->color = Color::BLACK;
                        w->color = Color::RED;
                        RightRotate(w);
                        w = x->p->right;
                    }
                    w->color = x->p->color;
                    x->p->color = Color::BLACK;
                    w->right->color = Color::BLACK;
                    LeftRotate(x->p);
                    x = this->root;
                }
            } else {
                w = x->p->left;
                if (w->color == Color::RED) {
                    w->color = Color::BLACK;
                    x->p->color = Color::RED;
                    RightRotate(x->p);
                    w = x->p->left;
                }
                if (w->right->color == Color::BLACK && w->left->color == Color::BLACK) {
                    w->color = Color::RED;
                    x = x->p;
                } else {
                    if (w->left->color == Color::BLACK) {
                        w->right->color = Color::BLACK;
                        w->color = Color::RED;
                        LeftRotate(w);
                        w = x->p->left;
                    }
                    w->color = x->p->color;
                    x->p->color = Color::BLACK;
                    w->left->color = Color::BLACK;
                    RightRotate(x->p);
                    x = this->root;
                }
            }
        }
        x->color = Color::BLACK;
    }
    
    RbNode<T>* Search(T key, RbNode<T> *x)
    {
        while (x != RbNode<T>::NIL && x->key != key)
            x = comp(key, x->key) ? x->left : x->right;
        return x;
    }

    RbNode<T>* Search(T key)
    {
        return Search(key, this->root);
    }
    
    RbNode<T>* Minimum(RbNode<T> *x)
    {
        while (x->left != RbNode<T>::NIL)
            x = x->left;
        return x;
    }

    RbNode<T>* Minimum()
    {
        return Minimum(this->root);
    }
    
    RbNode<T>* Maximum(RbNode<T> *x)
    {
        while (x->right != RbNode<T>::NIL)
            x = x->right;
        return x;
    }

    RbNode<T>* Maximum()
    {
        return Maximum(this->root);
    }
    
    RbNode<T>* Successor(RbNode<T> *x)
    {
        if (x->right != RbNode<T>::NIL)
            return Minimum(x->right);
        RbNode<T> *y = x->p;
        while (y != RbNode<T>::NIL && x == y->right)
            x = y, y = y->p;
        return y;
    }
    
    RbNode<T>* Predecessor(RbNode<T> *x)
    {
        if (x->left != RbNode<T>::NIL)
            return Maximum(x->left);
        RbNode<T> *y = x->p;
        while (y != RbNode<T>::NIL && x == y->left)
            x = y, y = y->p;
        return y;
    }

    const RbNode<T>* Root() const
    {
        return this->root;
    }

    void Deallocate(RbNode<T> *x)
    {
        if (x == RbNode<T>::NIL)
            return;
        Deallocate(x->left);
        Deallocate(x->right);
        delete x;
    }

    ~RbTree()
    {
        Deallocate(this->root);
    }
    
private:
    RbNode<T> *root {RbNode<T>::NIL};
    Compare comp{};
};

template<typename T>
RbNode<T> RbNode<T>::_NIL_NODE = RbNode{T{}, Color::BLACK};

template<typename T>
RbNode<T> *RbNode<T>::NIL = &_NIL_NODE;

#endif  // RbTree_hpp
