#include <iostream>
#include "B_Tree.h"

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
