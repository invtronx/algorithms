#ifndef FibHeap_hpp
#define FibHeap_hpp

#include "List.hpp"

//
// A Node of a Fibonacci Heap. Contains a key attribute that determines its
// position within the tree, a degree attribute that stores the number of
// children the node has, a marked attribute useful for CUT and CASCADING-CUT
// procedures used in DECREASE-KEY, a circular doubly-linked list of its
// children, as well as its corresponding linked list node by which the tree
// is connected.
//
struct FibNode {
    int key;
    int degree {0};
    bool marked {false};
    FibNode *parent {nullptr};
    List<FibNode*> children;
    ListNode<FibNode*> *lnode;
    
    FibNode(int _key = 0) : key{_key}, lnode{new ListNode<FibNode*>(this)} {}
};

//
// A Fibonacci Heap is a data structure that supports the mergeable heap
// operations, as well as the operations DECREASE-KEY and DELETE. Many of
// the operations supported by a F. heap run in constant amortized time, making
// them well-suited for applications that frequently invoke them. Fibonacci
// heaps have better asymptotic time bounds than binary heaps in their INSERT, 
// DECREASE-KEY and UNION operations, each of which run in amortized O(1).
//
// Fibonacci Heaps have different use cases from binary heaps. They are mostly
// used in an amortized setting, where the total cost is much more important
// than per-operation worst-case costs. Examples include Prim's Algorithm for 
// finding minimum spanning trees and Djikstra's algorithm for single-source
// shortest paths.
//
class FibHeap {
public:
    //
    // Inserts a node into the fibonacci heap by simply appending to the
    // root list. Consecutive insertions will result in a fibonacci heap
    // resembling a linear chain of nodes.
    //
    // Time Complexity: Amortized O(1)
    //
    void Insert(FibNode* fnode);

    //
    // Inserts a node with the given value into the fibonacci heap. Provides
    // an additional layer of abstraction over the standard INSERT.
    //
    // Time Complexity: Amortized O(1)
    //
    void InsertVal(int x);

    //
    // Merges two fibonacci heaps by simply joining their root lists. The
    // unioned node is destroyed in the process.
    //
    // Time Complexity: Amortized O(1)
    //
    void Union(FibHeap&& h);

    //
    // Extracts the node with the minimum key from the fibonacci heap. The
    // extracted minimum node retains all of its properties. The deallocation
    // of the returned node should be properly handled by the user.
    //
    // Time Complexity: Amortized O(lg n)
    //
    FibNode* ExtractMin();

    //
    // Extracts the node with the minimum key and returns its key. Provides
    // an additional layer of abstraction over the standard EXTRACT-MIN.
    //
    // Time Complexity: Amortized O(lg n)
    //
    int ExtractMinVal();

    //
    // Decreases the key of a particular node.
    //
    // Time Complexity: Amortized O(1)
    //
    void DecreaseKey(FibNode *fnode, int decreasedKey);

    //
    // Recursively deallocates all the nodes of the fibonacci heap.
    //
    ~FibHeap();

    // Accessors

    List<FibNode*>& rootList();

    int size() const;
    bool empty() const;

    const FibNode* min() const;
    int minVal() const;

private:
    int size_ {0};  // contains the total count of nodes in the heap

    FibNode *min_ {nullptr};  // contains the node with minimum key

    List<FibNode*> rootList_;  // chains the roots of subtrees in a linked list

    void FreeNodes(FibNode *root);
    void Consolidate();
    void Link(FibNode *child, FibNode *parent);
    void Cut(FibNode *child, FibNode *parent);
    void CascadingCut(FibNode *fnode);
};

#endif  /* FibHeap_hpp */

