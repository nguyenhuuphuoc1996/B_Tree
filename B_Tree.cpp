#include "B_Tree.h"

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

bool B_TREE::B_TREE_IS_CHILD_EXISTING_KEY(Node *node_x, int *index)
{
    bool bt_isChildHasKey = false;
    return bt_isChildHasKey;
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
                Node *bt_node = r->itsChildren[i]->itsChildren[preIndex];

                // Recusively delete node at preIndex
                //
                //
                B_TREE_DELETE(bt_node, preIndex);
            }
            // Case 2b
            else if (i <= r->n - 1 && r->itsChildren[i + 1]->n == MINIMUM_DEGREE_T)
            {
                int preIndex = 0;
                //
                // Move one key up to its parent
                r->key[i] = r->itsChildren[i + 1]->key[preIndex];
                // Back up node that just has been moved
                Node *bt_node = r->itsChildren[i + 1]->itsChildren[preIndex];

                // Recusively delete node at preIndex
                //
                //
                B_TREE_DELETE(bt_node, preIndex);
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
                // Do nothing
            }
        }
    }
    // Case 3: The search arrives at an internal node x that does not contain key k
    else
    {
        if (bt_isLeaf == true)
        {
            // Having no key
        }
        else
        {
            bool bt_isChildHasKey = false;
            int bt_IndexFoundKey = 0;
            // Put attention here
            if (k > r->key[i])
            {
                i += 1;
            }

            //
            bt_isChildHasKey = B_TREE_IS_CHILD_EXISTING_KEY(r->itsChildren[i], &bt_IndexFoundKey);
            if (bt_isChildHasKey == true && r->itsChildren[i]->n == MINIMUM_DEGREE_T - 1)
            {
                // Case 3a: x:c i has only t - 1 keys but has an immediate sibling with at least t keys
                if (bt_IndexFoundKey >= 1 && r->itsChildren[bt_IndexFoundKey + 1]->n >= MINIMUM_DEGREE_T)
                {
                    // Shift child's key to make room for x key
                    int k = 0;
                    for (k = 0; k < r->itsChildren[i]->n - bt_IndexFoundKey - 1; k++)
                    {
                        r->itsChildren[i]->key[bt_IndexFoundKey + k] = r->itsChildren[i]->key[bt_IndexFoundKey + 1 + k];
                        r->itsChildren[i]->itsChildren[bt_IndexFoundKey + k] = r->itsChildren[i]->itsChildren[bt_IndexFoundKey + 1 + k];
                    }
                    {
                        // Last child
                        r->itsChildren[i]->itsChildren[bt_IndexFoundKey + k] = r->itsChildren[i]->itsChildren[bt_IndexFoundKey + 1 + k];
                    }
                    // Move key
                    r->itsChildren[i]->key[r->itsChildren[i]->n - 1] = r->key[i];
                    r->key[i] = r->itsChildren[bt_IndexFoundKey + 1]->key[0];
                    // And then move it's child pointer
                    r->itsChildren[i]->itsChildren[r->itsChildren[i]->n] = r->itsChildren[bt_IndexFoundKey + 1]->itsChildren[0];

                    // Re-arrange x.c i+1
                    for (k = 0; k < r->itsChildren[bt_IndexFoundKey + 1]->n; k++)
                    {
                        r->itsChildren[bt_IndexFoundKey + 1]->key[k] = r->itsChildren[bt_IndexFoundKey + 1]->key[k + 1];
                        r->itsChildren[bt_IndexFoundKey + 1]->itsChildren[k] = r->itsChildren[bt_IndexFoundKey + 1]->itsChildren[k + 1];
                    }
                    {
                        // Last child
                        r->itsChildren[bt_IndexFoundKey + 1]->itsChildren[k] = r->itsChildren[bt_IndexFoundKey + 1]->itsChildren[k + 1];
                    }
                    // Decrease current node since that node is move up to node x: first node
                    r->itsChildren[bt_IndexFoundKey + 1]->n -= 1;
                }
                else if (bt_IndexFoundKey >= 1 && r->itsChildren[bt_IndexFoundKey - 1]->n >= MINIMUM_DEGREE_T)
                {
                    // Shift child's key to make room for x key
                    int k = 0;
                    for (k = bt_IndexFoundKey; k > 0; k--)
                    {
                        r->itsChildren[i]->key[k] = r->itsChildren[i]->key[k - 1];
                        r->itsChildren[i]->itsChildren[k] = r->itsChildren[i]->itsChildren[k - 1];
                    }
                    {
                        // Last child
                        r->itsChildren[i]->itsChildren[k] = r->itsChildren[i]->itsChildren[k - 1];
                    }
                    // Move key
                    r->itsChildren[i]->key[0] = r->key[i];
                    r->key[i] = r->itsChildren[bt_IndexFoundKey - 1]->key[r->itsChildren[bt_IndexFoundKey - 1]->n - 1];
                    // And then move it's child pointer
                    r->itsChildren[i]->itsChildren[0] = r->itsChildren[bt_IndexFoundKey - 1]->itsChildren[r->itsChildren[bt_IndexFoundKey - 1]->n];

                    // Re-arrange x.c i-1
                    // No need since it is last node, just decrease current node by one, like following step
                    // Decrease current node since that node is move up to node x: last node
                    r->itsChildren[bt_IndexFoundKey - 1]->n -= 1;
                }
                // Case 3b: x:c i and each of x:c i ’s immediate siblings have t - 1 keys
                else
                {
                }
            }
            else
            {
                B_TREE_DELETE(r->itsChildren[i], k);
            }
        }
    }
}
