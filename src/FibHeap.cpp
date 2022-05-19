#include <vector>
#include "FibHeap.hpp"

void FibHeap::Insert(FibNode *fnode)
{
    rootList_.Insert(fnode->lnode);
    if (min_ == nullptr || fnode->key < min_->key)
        min_ = fnode;
    ++size_;
}

void FibHeap::InsertVal(int x)
{
    FibNode *fnode = new FibNode(x);
    Insert(fnode);
}

void FibHeap::Union(FibHeap&& h)
{
    min_ = h.min_->key < min_->key ? h.min_ : min_;
    size_ += h.size_;
    rootList_.Union(std::move(h.rootList_));

    h.size_ = 0;
    h.rootList_.clear();
    h.min_ = nullptr;
}

FibNode* FibHeap::ExtractMin()
{
    FibNode *z = min_;
    if (z != nullptr) {
        for (FibNode *fn : z->children) {
            rootList_.Insert(fn->lnode);
            fn->parent = nullptr;
        }
        z->children.clear();
        rootList_.Delete(z->lnode);
        if (rootList_.empty()) {
            min_ = nullptr;
        } else {
            min_ = rootList_.head()->data;
            Consolidate();
        }
        --size_;
    }
    return z;
}

int FibHeap::ExtractMinVal()
{
    FibNode *z = ExtractMin();
    return z ? z->key : 0;
}

void FibHeap::DecreaseKey(FibNode *fnode, int decreasedKey)
{
    if (decreasedKey >= fnode->key)
        return;
    fnode->key = decreasedKey;
    FibNode* parent = fnode->parent;
    if (parent != nullptr && fnode->key < parent->key) {
        Cut(fnode, parent);
        CascadingCut(parent);
    }
    if (fnode->key < min_->key)
        min_ = fnode;
}

FibHeap::~FibHeap()
{
    // Not necessary to free the linked lists as they are statically allocated.
    for (FibNode *root : rootList_)
        FreeNodes(root);
}

void FibHeap::FreeNodes(FibNode* root)
{
    // Should work even if root has no children
    for (FibNode *child : root->children)
        FreeNodes(child);
    delete root;  // deallocate the FibNode object
}

List<FibNode*>& FibHeap::rootList()
{
    return rootList_;
}

int FibHeap::size() const
{
    return size_;
}

bool FibHeap::empty() const
{
    return size_ == 0;
}

const FibNode* FibHeap::min() const
{
    return min_;
}

int FibHeap::minVal() const
{
    // Users should check for the nullity of min_ beforehand.
    return min_->key;  
}

void FibHeap::Consolidate()
{
    const int LG_SIZE = 32 - __builtin_clz(size_);
    const int MAX_DEGREE = 8 * LG_SIZE * LG_SIZE;
    std::vector<FibNode*> degree(MAX_DEGREE, nullptr);
    // Problem here: elements in the linked list get deleted during insertion
    for (FibNode *root : rootList_) {
        FibNode* x = root;
        int lastDegree;
        for (int d = x->degree; degree[lastDegree = d] != nullptr; d++) {
            FibNode *y = degree[d];
            if (y->key < x->key)
                std::swap(x, y);
            Link(y, x);
            degree[d] = nullptr;
        }
        degree[lastDegree] = x;
    }
    min_ = nullptr;
    rootList_.clear();
    for (int d = 0; d < degree.size(); d++) {
        if (degree[d] == nullptr) continue;
        rootList_.Insert(degree[d]->lnode);
        if (min_ == nullptr || degree[d]->key < min_->key)
            min_ = degree[d];
    }
}

void FibHeap::Link(FibNode *child, FibNode *parent)
{
    rootList_.Delete(child->lnode);
    parent->children.Insert(child->lnode);
    ++parent->degree;
}

void FibHeap::Cut(FibNode *child, FibNode *parent)
{
    parent->children.Delete(child->lnode);
    --parent->degree;
    rootList_.Insert(child->lnode);
    child->parent = nullptr;
    child->marked = false;
}

void FibHeap::CascadingCut(FibNode *fnode)
{
    FibNode *parent = fnode->parent;
    if (parent != nullptr) {
        if (fnode->marked) {
            Cut(fnode, parent);
            CascadingCut(parent);
        } else {
            fnode->marked = true;
        }
    }
}

