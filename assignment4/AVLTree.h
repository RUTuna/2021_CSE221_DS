#include "Tree.h"

template <class keyT, class valT>
class AVLTree_t : public Tree_t<keyT, valT> {
    long check_bf(Node_t<keyT, valT>* n) {
        // Checks balance factor values in tree.
        long hr, hl, h;
        hr = hl = 0;
        if(!n) return 0;
        hr = check_bf(n->right);
        hl = check_bf(n->left);
        if(hr > hl) h = hr;
        else h = hl;
        h += 1;
        long bf = hl - hr;
        assert(bf == n->meta);
        assert(bf < 2);
        assert(bf > -2);
        return h;
    }

    long set_bf(Node_t<keyT, valT>* n) {
        long hr, hl, h;
        hr = hl = 0;
        if(!n) return 0;
        hr = set_bf(n->right);
        hl = set_bf(n->left);
        if(hr > hl) h = hr;
        else h = hl;
        h += 1;
        long bf = hl - hr;
        n->meta = bf;
        return h;
    }

    // Optional private functions.
    // These are here to guide you, but you can just ignore these.
    Node_t<keyT, valT>* balance(Node_t<keyT, valT>* a) {
        // Make tree balanced
        if(a->meta > 1){ // LL or LR
            if(a->left->meta < 0) { Tree_t<keyT, valT>::rotate(a->left, false);} // Only LR, Lrotate at a's Lchild
            Tree_t<keyT, valT>::rotate(a, true); // LR, LL both, Rroate at a

        } else if(a->meta < -1){ // RR or RL
            if(a->right->meta > 0) {Tree_t<keyT, valT>::rotate(a->right, true);} // Only RL, Rrotate at a's Rchild
            Tree_t<keyT, valT>::rotate(a, false); // LR, LL both, Rroate at a
        } else if(a->parent == nullptr) { // root면 종료  
            return a;
        } else balance(a->parent);
    }

    // Node_t<keyT, valT>* insert_internal(keyT key, valT value) {
    //     // Helper function for insert()
    // }

    // void remove_node(Node_t<keyT, valT>* n) {
    //     // Helper function for remove()
    // }

public:
    void insert(keyT key, valT value) {
        // If there is no node that has the given key,
        // create a new one, place it in the right place, and store the value.
        // If there already is a node that has the given key,
        // update the value, rather than making a new one.
        Node_t<keyT, valT>* root = Tree_t<keyT, valT>::root;
        Node_t<keyT, valT>* n = Tree_t<keyT, valT>::insert_internal(key,value);
        set_bf(root);
        balance(n);
        root = Tree_t<keyT, valT>::root;
        set_bf(root);
    }

    bool remove(keyT key) {
        // Find the node that has the given key and remove that node.
        Node_t<keyT, valT>* root = Tree_t<keyT, valT>::root;
        if(!root) return false;
        Node_t<keyT, valT>* n = Tree_t<keyT, valT>::search_subtree(root,key);
        Node_t<keyT, valT>* np = nullptr;
        if(n && n->key == key){
            if(n!=root) np = n->parent;
            Tree_t<keyT, valT>::remove_node(n);
            root = Tree_t<keyT, valT>::root;
            set_bf(root);
            if(n!=root) balance(np);
            return true;
        } else return false;

    }
};
