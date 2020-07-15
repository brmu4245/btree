// btree.cpp

#include <iostream>
#include "btree.h"

using namespace std;

void insert(btree*& root, int key) {
    std::cout<<"Number to be inserted = " << key << std::endl;
    std::cout<<"root->keys[0] = " << root->keys[0] << std::endl;
    std::cout<<"root->keys[1] = " << root->keys[1] << std::endl;
    std::cout<<"root->keys[2] = " << root->keys[2] << std::endl;
    std::cout<<"root->keys[3] = " << root->keys[3] << std::endl;
    std::cout<<"root->num_keys = " << root->num_keys << std::endl;
    std::cout<<"root->children[0] = " << root->children[0] << std::endl;
    std::cout<<"root->children[1] = " << root->children[1] << std::endl;
    std::cout<<"root->children[2] = " << root->children[2] << std::endl;
    std::cout<<"root->children[3] = " << root->children[3] << std::endl;
    std::cout<<"root->children[4] = " << root->children[4] << std::endl;
    btree* r = root;
    if (root->num_keys == BTREE_ORDER-1){
        btree* s = new btree;
        s->is_leaf = true;
        s->num_keys = 0;
        for (int i=0; i <= BTREE_ORDER-1; i++) {
            s->children[i] = NULL;
        }
        root = s;
        s->is_leaf = false;
        s->num_keys = 0;
        s->children[0] = r;
        split_child(s, 0);
        insert_nonfull(s, key);
    } else {
        insert_nonfull (r, key);
    }
    std::cout<<"final root->keys[0] = " << root->keys[0] << std::endl;
    std::cout<<"final root->keys[1] = " << root->keys[1] << std::endl;
    std::cout<<"final root->keys[2] = " << root->keys[2] << std::endl;
    std::cout<<"final root->keys[3] = " << root->keys[3] << std::endl;
    std::cout<<"final root->num_keys = " << root->num_keys << std::endl;
    std::cout<<"final child0->keys[0] = " << root->children[0]->keys[0] << std::endl;
    std::cout<<"final child0->keys[1] = " << root->children[0]->keys[1] << std::endl;
    std::cout<<"final child0->keys[2] = " << root->children[0]->keys[2] << std::endl;
    std::cout<<"final child0->keys[3] = " << root->children[0]->keys[3] << std::endl;
    std::cout<<"final child0->num_keys = " << root->children[0]->num_keys << std::endl;
    std::cout<<"final child1->keys[0] = " << root->children[1]->keys[0] << std::endl;
    std::cout<<"final child1->keys[1] = " << root->children[1]->keys[1] << std::endl;
    std::cout<<"final child1->keys[2] = " << root->children[1]->keys[2] << std::endl;
    std::cout<<"final child1->keys[3] = " << root->children[1]->keys[3] << std::endl;
    std::cout<<"final child1->num_keys = " << root->children[1]->num_keys << std::endl;
}

void split_child(btree*& root, int i) { // i = index of full child of root
    btree* z = new btree;
    z->is_leaf = true;
    std::cout << std::boolalpha;
    z->num_keys = 0;
    for (int i=0; i <= BTREE_ORDER-1; i++) {
        z->children[i] = NULL;
    }
    btree* y = root->children[i];
    z->is_leaf = y->is_leaf;
    z->num_keys = 1;
    for (int j = 0; j<1; j++) {
        z->keys[j] = y->keys[j+3];
    }
    if (y->is_leaf == false) {
        for (int j = 1; j > 3; j++) {
            z->children[j] = y->children[j + 3];
        }
    }
    y->num_keys = 2;
    for (int j = root->num_keys + 1; j >= i+1; j--) {
        root->children[j+1] = root->children[j];
    }
    root->children[i+1] = z;
    for (int j = root->num_keys; j >= i; j--) {
        root->keys[j+1] = root->keys[j];
    }
    root->keys[i] = y->keys[2];
    root->num_keys = root->num_keys + 1;
}
//Takes in the btree from insert above with the number k to insert into
//the tree.
void insert_nonfull(btree*& root, int k) {
    int i = root->num_keys;
    if (root->is_leaf == true) {                //If the node is a leaf...
        while (i >= 1 && k < root->keys[i-1]) { //move numbers over to make 
            root->keys[i] = root->keys[i-1];    //room for new number
            i--;
        }
        root->keys[i] = k;                      //inserts new number
        root->num_keys++;
    } else {                                    //If node is not a leaf...
        while (i >= 1 and k < root->keys[i-1]){
            i--;
        }
        root = root->children[i];               //...move to appropriate child.
        if (root->num_keys == BTREE_ORDER-1) {
            split_child(root, i);               //If child is full, split again
            if (k > root->keys[i]) {
                i++;
            }
        }
        insert_nonfull(root, k);
        
    }
}

void remove(btree*& root, int key) {
    
}
// find locates the node that either: (a) currently contains this key,
// or (b) the node that would contain it if we were to try to insert
// it.  Note that this always returns a non-null node.
btree* find(btree*& root, int key) {
    int i = 0;
    while (i <= BTREE_ORDER && key > root->keys[i]) {
        i++;
    }
    if (i <= BTREE_ORDER && key == root->keys[i]) {
        return root;
    } else if (root->is_leaf == true) {
        return NULL;
    } else {
        root = root->children[i];
        return find(root, key);
    }
}
// count_nodes returns the number of nodes referenced by this
// btree. If this node is NULL, count_nodes returns zero; if it is a
// root, it returns 1; otherwise it returns 1 plus however many nodes
// are accessable via any valid child links.
int count_nodes(btree*& root) {
    int counter = 1;
    if (root == NULL) {
        return counter;
    }
    for (int i = 0; i < BTREE_ORDER + 1; i++) {
        if (root->children[i] != NULL) {
            counter++;
            if (root->children[i]->children[i] != NULL) {
                for (int k = 0; k < BTREE_ORDER + 1; k++) {
                    if (root->children[i]->children[k] != NULL) {
                        counter++;
                    }
                }
            }
        }
    }
    return counter; // TODO
}
// count_keys returns the total number of keys stored in this
// btree. If the root node is null it returns zero; otherwise it
// returns the number of keys in the root plus however many keys are
// contained in valid child links.
int count_keys(btree*& root) {
    int counter = 0;
    if (root->num_keys == NULL) {
        return counter;
    }
    for (int i = 0; i < BTREE_ORDER+1; i++) {
        if (root->keys[i] != NULL) {
            counter++;
        }
    }
//    for (int i = 1; i < BTREE_ORDER+1; i++) {
//        root = root->children[i];
//        for (int k = 0; k < BTREE_ORDER; k++) {
//            if (root->keys[k] != NULL) {
//                counter ++;
//            }
//        }
//    }
//                for (int k = 0; k < BTREE_ORDER + 1; k++) {
//                    if (root->children[i]->children[k] != NULL) {
//                        counter++;
//                    }
//                }
//            }
        
    

    std::cout<< "The number of keys are: " << counter << std::endl;
    return counter; // TODO
}

