#include "Tree.h"

#define BLACK 0
#define RED 1

template <class keyT, class valT>
class RBTree_t : public Tree_t<keyT, valT> {
    size_t check_rb(Node_t<keyT, valT>* n) {
        // Checks RB tree property
        size_t b, br, bl;
        br = bl = b = 1;
        assert(n);
        if(n && n->right) {
            br = check_rb(n->right);
            assert(n->meta == BLACK || n->right->meta == BLACK);
        }
        if(n && n->left) {
            bl = check_rb(n->left);
            assert(n->meta == BLACK || n->left->meta == BLACK);
        }
        b = br;
        if(n == this->root && br != bl) {
            //dump_subtree(this->root);
        }
        assert(n != this->root || br == bl);
        b = br;
        if(n->meta == BLACK) b += 1;
        if(n == this->root) assert(n->meta == BLACK);

        return b;
    }

    // Optional private functions.
    // These are here to guide you, but you can just ignore these.
    void insert_balance(Node_t<keyT, valT>* n) {
        // Make tree balanced
        if(n->parent == nullptr) return; // root면 종료
        Node_t<keyT, valT>* pp = n->parent;
        if(pp->parent == nullptr) return; // root의 자식이면 종료
        Node_t<keyT, valT>* gp = pp->parent;
        assert(gp != nullptr);
        Node_t<keyT, valT>* d = nullptr; // gp의 다른 child

        if(pp == gp->left) d = gp->right; // pp가 gp의 Lchild
        else d = gp->left; // pp가 gp의 Rchild일 떄
        bool d_black = true;
        if(d == nullptr) d_black = true;
        else if(d->meta == RED) d_black = false;
        // assert(d != nullptr);

        if(n->meta == RED && pp->meta == RED){ // 연속된 Rnode
            if(!d_black){ // XYr
                flip(gp);
                flip(pp);
                flip(d);
                insert_balance(gp);
            } else if(pp == gp->left){ // LL or LR
                if(n == pp->left){ // LL
                    flip(gp);
                    flip(pp);
                    Tree_t<keyT, valT>::rotate(gp, true); // Rroate at gp
                } else { // LR
                    flip(gp);
                    flip(n);
                    Tree_t<keyT, valT>::rotate(pp, false); // Lroate at pp
                    Tree_t<keyT, valT>::rotate(gp, true); // Rroate at gp
                }
            } else if(pp == gp->right){ // RR or RL
                if(n == pp->right){ // RR
                    flip(gp);
                    flip(pp);
                    Tree_t<keyT, valT>::rotate(gp, false); // Lrotate at gp
                } else { // RL
                    flip(gp);
                    flip(n);
                    Tree_t<keyT, valT>::rotate(pp, true); // Rroate at pp
                    Tree_t<keyT, valT>::rotate(gp, false); // Lroate at gp
                }
            }
        } else return;
    }

    void flip(Node_t<keyT, valT>* n) {
        // Flip the color of this node and all children
        if(n->parent == nullptr) return; // root면 안바꿔
        if(n->meta == RED) n->meta = BLACK;
        else n->meta = RED;
    }

    bool remove_balance(Node_t<keyT, valT>* y, bool direction) {
        Node_t<keyT, valT>* x = direction ? y->right : y->left; // n's child (now y's child)
        if(x->meta == RED) { // n의 child가 red면
            flip(x);
            return true; // 색만 바꾸고 종료
        }

        // n의 child도 BLACK 일 때
        Node_t<keyT, valT>* v = direction ? y->left : y->right; // y's another child
        Node_t<keyT, valT>* w = direction ? v->left : v->right; // x랑 먼 v's child
        Node_t<keyT, valT>* z = direction ? v->right : v->left; // x랑 가까운 v's child
        assert(v != nullptr && w != nullptr && z != nullptr);

        if(v->meta == RED && y->meta == BLACK){ // case 1
            flip(y);
            flip(v);
            Tree_t<keyT, valT>::rotate(y, direction); // left면 Lrotate
            return remove_balance(y, direction); // case 2,3,4 중으로 가길 바란다
        } else if(v->meta == BLACK) {
            if(z->meta == BLACK && w->meta == BLACK){ // case 0 or case 2
                flip(v);
                if(y->meta == RED){ // case 2
                    flip(y);
                    return true;
                } else { // case 0
                    assert(y->parent != nullptr);
                    bool dire = (y == y->parent->right);
                    return remove_balance(y->parent, dire);
                }
            }

            if(w->meta == RED){ // case 4
                short y_color = y->meta;
                y->meta = v->meta;
                v->meta = y_color; // y랑 v 색 교환
                flip(w);
                Tree_t<keyT, valT>::rotate(y, direction); // left면 Lrotate
                return true;
            } else if(z->meta == RED && w->meta == BLACK){ // case 3
                flip(v);
                flip(z);
                Tree_t<keyT, valT>::rotate(v, !direction); // left면 Rrotate
                return remove_balance(y, direction); // case 4로 가길 바란다
            }
        }
        return true; // exist니까
    }


public:
    void insert(keyT key, valT value) {
        // If there is no node that has the given key,
        // create a new one, place it in the right place, and store the value.
        // If there already is a node that has the given key,
        // update the value, rather than making a new one.
        Node_t<keyT, valT>* root = Tree_t<keyT, valT>::root;
        Node_t<keyT, valT>* n = (root == nullptr ? nullptr : Tree_t<keyT, valT>::search_subtree(root,key));
        bool isExist = false;
        if(n && n->key == key) isExist = true; 
        n = Tree_t<keyT, valT>::insert_internal(key,value);
        if(isExist) return; // 존재하는 key 였으면 값만 교체하고 pass
        else if(n->parent == nullptr) n->meta = BLACK; // root면 BLACK으로 
        else {
            n->meta = RED; // 그 외는 RED
            if(n->parent->meta == BLACK) return; // 추가한 노드의 부모가 BLACK이면 통과 (부모가 root거나 그 외)
            else { // 그 외는 balace
                insert_balance(n);
            }
        }
    }


    bool remove(keyT key) {
        // Find the node that has the given key and remove that node.
        Node_t<keyT, valT>* root = Tree_t<keyT, valT>::root;
        if(!root) return false;
        Node_t<keyT, valT>* n = Tree_t<keyT, valT>::search_subtree(root,key);
        if(n && n->key == key){ // exist
            if(n->parent == nullptr){ // n이 root
                Tree_t<keyT, valT>::remove_node(n);
                root = Tree_t<keyT, valT>::root;
                root->meta = BLACK; // 제거 후 BLACK 지정
                return true;
            } else if(n->left && n->right){ // n이 two children을 가질 때
                if(n->meta == RED){ // n이 RED
                    short min_color = Tree_t<keyT, valT>::get_min(n->right)->meta; // n 자리에 올 node의 기존 색 저장
                    Tree_t<keyT, valT>::remove_node(n); // n의 meta값은 유지, 즉 min_node의 key, value만 가져옴
                    if(min_color == BLACK) n->right->meta = BLACK; // 기존 색이 BLACK 이었다면, BLACK 수가 줄어들었으니 Rchild를 BLACK으로
                    return true;
                } else { // n이 BLACK
                    Node_t<keyT, valT>* y = n->parent; // n's parent
                    bool direction = (n == y->right); // n이 Rchild면 true, Lchile면 false
                    Tree_t<keyT, valT>::remove_node(n);
                    return remove_balance(y, direction); // 5가지 case에 따라 처리
                }
            } else { // n이 one or non child를 가질 때
                if(n->meta == RED){ // n이 RED
                    short min_color = Tree_t<keyT, valT>::get_min(n->right)->meta; // n 자리에 올 node의 기존 색 저장
                    Tree_t<keyT, valT>::remove_node(n);
                    n->meta = min_color; // 기존 색 유지
                } else Tree_t<keyT, valT>::remove_node(n); // n이 BLACK이면 그냥 삭제 (자동으로 BLACK으로 변경)
                return true;
            }
        }
        return false; // not exist
    }

};
