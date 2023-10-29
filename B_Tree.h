#ifndef B_TREE_H
#define B_TREE_H
#define MINIMUM_DEGREE_T 3

// B-Tree atributes
//
//
// 1. x.n current keys in node
// 2. Keys in node is ordered in increasing order
// 3. Every node has x.n keys => has x.n + 1 children called x.c1, x.c2 ... x.c(n+1)
// 4. LEAF: true is internal node, false is leaf
// 5. Important param t with t >= 2
//  For internal node other than the root
//  5.1. (t - 1) <= x.n <= 2t - 1
//  5.2. t <= children <= 2t
struct Node
{
    int n;                                       // Current key in Node
    int key[2 * MINIMUM_DEGREE_T - 1 + 1];       // Every key value
    Node *itsChildren[2 * MINIMUM_DEGREE_T + 1]; // Node's children
    bool LEAF;                                   // Root, internal node or leaf
};

struct SearchNode
{
    Node *node; // Address of node that contains key k
    int i;      // Index at which key is k
};

class B_TREE
{
public:
    B_TREE() = default;
    ~B_TREE() {}
    SearchNode B_TREE_SEARCH(Node *node, int k);
    void B_TREE_CREATE();
    void B_TREE_INSERT(int k);
    void B_TREE_DELETE(Node *p_node, int k);

private:
    void B_TREE_SPLIT_CHILD(Node *node_x, int i);
    void B_TREE_INSERT_NONFULL(Node *node, int k);
    Node *B_TREE_SPLIT_ROOT();
    bool B_TREE_IS_CHILD_EXISTING_KEY(Node *node_x, int *index);

private:
    Node *root;
};
#endif
