#include <iostream>

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
    void B_TREE_MOVE_NODE_UP(Node *node_x, int index);

private:
    Node *root;
};

SearchNode B_TREE::B_TREE_SEARCH(Node *node, int k)
{
    SearchNode retNode;
    int i = 1;
    while ((i <= node->n) && (k > node->key[i]))
    {
        i += 1;
    }
    // Looking for current node
    // Return a pair of index and address or node
    if (i <= node->n && k == node->key[i])
    {
        retNode.i = i;
        retNode.node = node->itsChildren[i];
    }
    // If node is leaf and there is no matching node
    // Return null pointer
    else if (node->LEAF = true)
    {
        retNode.i = 0xff;
        retNode.node = nullptr;
    }
    // Searching down to it's children
    else
    {
        B_TREE_SEARCH(node->itsChildren[i], k);
    }
    return retNode;
}
void B_TREE::B_TREE_CREATE()
{
    Node *node = new Node;
    node->LEAF = true;
    node->n = 0;
    root = node;
}

// node x is not full, if full => cannot move x's child (medium key of y) into x
// y = x.ci is full node => calculate medium key => move medium key into node x
//
//
void B_TREE::B_TREE_SPLIT_CHILD(Node *node_x, int i)
{
    int j;
    // node_x is nonfull, i is child index to be split
    Node *y = node_x->itsChildren[i];
    // Node z will take half of full node

    // -------- z node part ----------
    //
    //
    Node *z = new Node;
    z->LEAF = y->LEAF;
    // Split y from <<(2t - 1) keys and (2t)>> children into two << (t - 1) keys and (t) children
    // The medium key will move up to x node
    z->n = MINIMUM_DEGREE_T - 1; // (t - 1) keys

    // (t - 1) keys
    for (j = 1; j <= MINIMUM_DEGREE_T - 1; j++)
    {
        // Z key will take the greatest nodes of y
        z->key[j] = y->key[MINIMUM_DEGREE_T + j];
    }

    // z will take children of y in case y is not LEAF
    if (y->LEAF == false)
    {
        // (t) children
        for (j = 1; j <= MINIMUM_DEGREE_T; j++)
        {
            z->itsChildren[j] = y->itsChildren[MINIMUM_DEGREE_T + j];
        }
    }

    // -------- y node part ----------
    //
    //
    // Just notify number of nodes in y
    // The rest of y would remain but un-valuable
    y->n = MINIMUM_DEGREE_T - 1;

    // --------- x node part ----------
    //
    //
    // Shift node's children to make room for z as a child
    for (j = node_x->n + 1; j >= i + 1; j--)
    {
        node_x->itsChildren[j + 1] = node_x->itsChildren[j];
    }
    // Insert z as a child of node x
    node_x->itsChildren[i + 1] = z;

    // Shift node's key to make room for medium key from y
    for (j = node_x->n; j > i; j--)
    {
        node_x->key[j + 1] = node_x->key[j];
    }
    node_x->key[i] = y->key[MINIMUM_DEGREE_T]; // medium key in current node
    node_x->n += 1;                            // Increase key of parent node by 1 because of additional node
}

Node *B_TREE::B_TREE_SPLIT_ROOT()
{
    // If root is full key
    // Then split the root into two nodes
    // s will become new root node
    Node *s = new Node;
    s->LEAF = false;
    s->n = 0;
    s->itsChildren[1] = root;
    root = s;
    B_TREE_SPLIT_CHILD(s, 1);
    return s;
}

void B_TREE::B_TREE_INSERT_NONFULL(Node *node_x, int k)
{
    int i = node_x->n;

    if (node_x->LEAF == true)
    {
        while (i >= 1 && k < node_x->key[i])
        {
            node_x->key[i + 1] = node_x->key[i]; // Shift key in node_x to make room for k
            i -= 1;
        }
        node_x->key[i + 1] = k; // Insert k in to node_x
        node_x->n += 1;
    }
    else
    {
        while (i >= 1 && k < node_x->key[i])
        {
            i -= 1;
        }
        i += 1; // Here is pointer gap between two key, where k belong to
        if (node_x->itsChildren[i]->n == 2 * MINIMUM_DEGREE_T - 1)
        {
            // Children has full node
            B_TREE_SPLIT_CHILD(node_x, i);
            if (k > node_x->key[i])
            {
                i += 1;
            }
        }
        B_TREE_INSERT_NONFULL(node_x->itsChildren[i], k);
    }
}

// The full implementation of B-Tree delete
//
//
//
void B_TREE::B_TREE_INSERT(int k)
{
    // Always search at root
    Node *r = root;
    // If root is full => split the root
    if (r->n == 2 * MINIMUM_DEGREE_T - 1)
    {
        // Split the root and then insertion process
        Node *s = B_TREE_SPLIT_ROOT();
        B_TREE_INSERT_NONFULL(s, k);
    }
    // Root is not full => insertion process
    else
    {
        B_TREE_INSERT_NONFULL(r, k);
    }
}

void B_TREE::B_TREE_MOVE_NODE_UP(Node *node_x, int index)
{
}

// The implementation of B-Tree deletion
//
//
// Hint: start call is ar root
void B_TREE::B_TREE_DELETE(Node *p_node, int k)
{
    Node *r = p_node;
    bool bt_isLeaf = false;
    int i = 0;
    while (i <= r->n - 1 && k > r->key[i])
    {
        i += 1;
    }
    // Case 1
    //
    //
    if (r->LEAF == true)
    {
        bt_isLeaf = true;
    }

    // Case 2: The search arrives at an internal node x that contains key k
    //
    //
    if (r->key[i] == k)
    {
        if (bt_isLeaf == true)
        {
            int x;
            // Re-arrange value
            for (x = i; x < r->n; x++)
            {
                r->key[x] = r->key[x + 1];
            }

            // Decrease current number of key by 1
            r->n = r->n - 1;
        }
        else
        {
            // Case 2a
            if (i <= r->n - 1 && r->itsChildren[i]->n == MINIMUM_DEGREE_T)
            {
                int preIndex = r->itsChildren[i]->n - 1;
                //
                // Move one key up to its parent
                r->key[i] = r->itsChildren[i]->key[preIndex];
                // Back up node that just has been moved
                Node *bt_node = r->itsChildren[i]->itsChildren[r->itsChildren[i]->n - 1];

                // Recusively delete node at preIndex
                //
                //
                B_TREE_DELETE(bt_node, i);
            }
            // Case 2b
            else if (i <= r->n - 1 && r->itsChildren[i + 1]->n == MINIMUM_DEGREE_T)
            {
                int preIndex = r->itsChildren[i + 1]->n - 1;
                //
                // Move one key up to its parent
                r->key[i] = r->itsChildren[i + 1]->key[preIndex];
                r->itsChildren[i + 1]->n -= 1;
            }
            // Case 2c
            else if (r->itsChildren[i]->n == MINIMUM_DEGREE_T - 1 && r->itsChildren[i + 1]->n == MINIMUM_DEGREE_T - 1)
            {
                int k = 0;
                // Delete key from current node
                //
                //
                for (k = i; k < r->n; k++)
                {
                    r->key[k] = r->key[k + 1];
                }
                // Decrease number of current key by 1
                r->n -= 1;

                // Merge it's children
                //
                //
                // Merge key
                r->itsChildren[i]->key[r->itsChildren[i]->n] = k;
                r->itsChildren[i]->n += 1;
                for (k = 0; k < r->itsChildren[i + 1]->n; k++)
                {
                    r->itsChildren[i]->key[r->itsChildren[i]->n + k] = r->itsChildren[i + 1]->key[k];
                }
                // Then merge children
                for (k = 0; k < r->itsChildren[i + 1]->n + 1; k++)
                {
                    r->itsChildren[i]->itsChildren[r->itsChildren[i]->n + k] = r->itsChildren[i + 1]->itsChildren[k];
                }
                B_TREE_DELETE(r->itsChildren[i], k);
            }
            else
            {
            }
        }
    }
}

int main()
{
    B_TREE btree1;
    btree1.B_TREE_CREATE();
    btree1.B_TREE_INSERT(1);
    btree1.B_TREE_INSERT(3);
    btree1.B_TREE_INSERT(4);
    btree1.B_TREE_INSERT(5);
    btree1.B_TREE_INSERT(7);
    btree1.B_TREE_INSERT(10);
    btree1.B_TREE_INSERT(11);
    btree1.B_TREE_INSERT(13);
    btree1.B_TREE_INSERT(14);
    btree1.B_TREE_INSERT(15);
    btree1.B_TREE_INSERT(16);
    btree1.B_TREE_INSERT(18);
    btree1.B_TREE_INSERT(19);
    btree1.B_TREE_INSERT(20);
    btree1.B_TREE_INSERT(21);
    btree1.B_TREE_INSERT(22);
    btree1.B_TREE_INSERT(24);
    btree1.B_TREE_INSERT(25);
    btree1.B_TREE_INSERT(26);

    std::cout << "------" << std::endl;
}
