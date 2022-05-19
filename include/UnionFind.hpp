#ifndef UnionFind_hpp
#define UnionFind_hpp

template<typename T>
struct Member {
    T key;
    int rank;
    Member *parent;

    Member(T _key) : key{_key}, rank{0}, parent{this} {}
};

template<typename T>
Member<T>* MakeSet(T x)
{
    return new Member<T>(x);
}

template<typename T>
Member<T>* FindSet(Member<T>* x)
{
    if (x != x->parent)
        x->parent = FindSet(x->parent);
    return x->parent;
}

template<typename T>
void Union(Member<T>* x, Member<T>* y)
{
    Member<T> *rx = FindSet(x), *ry = FindSet(y);
    // UNION(x, y) ==> y when x.rank == y.rank
    if (rx->rank > ry->rank) {
        ry->parent = rx;
    } else {
        rx->parent = ry;
        if (rx->rank == ry->rank)
            ++ry->rank;
    }
}

#endif  /* UnionFind_hpp */
