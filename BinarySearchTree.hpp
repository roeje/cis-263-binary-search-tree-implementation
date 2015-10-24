/**
 * Binary Search Tree (expanded definition)
 */

#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include <iostream>
#include <set>
#include <vector>

using namespace std;

template<typename T>

class BinarySearchTree {
private:
    /* private data */
    struct gv_node;
    /* forward declaration */
    gv_node *root;

public:
    BinarySearchTree() {
        root = nullptr;
    }

    ~BinarySearchTree() {
        removeAll(root);       /* start emptying the tree from the root */
    }

    //--- accessor functions ---//
    bool isEmpty() const {
        return root == nullptr;
    }

    bool contains(const T &x) const {
        return contains(x, root);
    }

    const T &findMin() const {
        return findMin(root)->data;
    }

    const T &findMax() const {
        return findMax(root)->data;
    }

    void print(std::ostream &destination = std::cout) const {
        print(destination, root);
    }

    //--- mutator functions ---//
    void insert(const T &x) {
        _insert(x, root);
    }

    void remove(const T &x) {
        remove(x, root);
    }

    int number_of_nodes() const {
        return _number_of_nodes(root);
    }

    int number_of_leaves() const {
        return _number_of_leaves(root);
    }

    int number_of_full_nodes() const {
        return _number_of_full_nodes(root);
    }

    /* Remove all the leaf nodes in the tree */
    std::set<T> remove_leaves() {
        std::set<T> col;
        auto point = &col;
        _remove_leaves(root, point);
        return col;
    }

    vector<T> get_range(const T &k1, const T &k2) const {
        vector<T> result;
        auto tempVec = &result;
        _get_range(tempVec, root, k1, k2);
        return result;
    }

private:
    /*!!! Notice how _insert and remove are const member functions !!! */

    void _insert(const T &x, gv_node *&t) const {
        if (t == nullptr) {
            t = new gv_node;
            t->data = x;
            t->left = t->right = nullptr;
        }
        else if (x < t->data)
            _insert(x, t->left);
        else if (x > t->data)
            _insert(x, t->right);
        else
            /* attempted to insert a duplicate item ... */
            ;
    }

    void remove(const T &x, gv_node *&t) const {
        if (t == nullptr)
            return;

        if (x < t->data)
            remove(x, t->left);
        else if (x > t->data)
            remove(x, t->right);
        else {
            /* we found the node to delete */
            if (t->left != NULL && t->right != NULL) // TWO children
            {
                /* find the smallest value in the right subtree,
                 * and replace the current node with that value */
                t->data = findMin(t->right)->data;
                remove(t->data, t->right);
            }
            else {              // one child or none
                gv_node *me = t;
                /* promote the child node */
                t = (t->left != NULL) ? t->left : t->right;
                delete me;
            }
        }
    }

    bool contains(const T &x, gv_node *t) const {
        if (t == nullptr)
            return false;

        if (x < t->data)
            return contains(x, t->left);
        if (x > t->data)
            return contains(x, t->right);
        return true;
    }

    void removeAll(gv_node *&t) {
        if (t == nullptr)
            return;
        removeAll(t->left);    /* empty the left sub-tree */
        removeAll(t->right);   /* empty the right subtree */
        delete t;               /* delete self */
        t = nullptr;
    }

    void print(std::ostream &dest, gv_node *t, int depth = 0) const {
        if (t == nullptr)
            return;
        for (int k = 0; k < depth - 1; k++) // for indentation
            dest << "|   ";
        if (depth > 0)
            dest << "+---";
        dest << t->data << std::endl;
        print(dest, t->left, depth + 1);
        print(dest, t->right, depth + 1);
    }

    gv_node *findMin(gv_node *t) const {
        if (t == nullptr)
            return nullptr;

        /* locate the leftmost node with no left child */
        while (t->left != nullptr)
            t = t->left;
        return t;
    }

    gv_node *findMax(gv_node *t) const {
        if (t == nullptr)
            return nullptr;

        /* locate the rightmost leaf node with no right child */
        while (t->right != nullptr)
            t = t->right;
        return t;
    }

    int _number_of_nodes(gv_node *t) const {
        /*Runtime is approximately O(N)*/
        if (t == nullptr)
            return 0;
        else {
            return (1 + _number_of_nodes(t->left) + _number_of_nodes(t->right));
        }
    }

    int _number_of_leaves(gv_node *t) const {
        /*Runtime is O(N)*/
        if (t == nullptr)
            return 0;
        if (t->right == nullptr && t->left == nullptr)
            return 1;
        else {
            return (_number_of_leaves(t->right) + _number_of_leaves(t->left));
        }
    }

    int _number_of_full_nodes(gv_node *t) const {
        /*Runtime O(N)*/
        if (t == nullptr)
            return 0;
        if (t->right != nullptr && t->left != nullptr)
            return (1 + _number_of_full_nodes(t->right) + _number_of_full_nodes(t->left));
        else {
            return (_number_of_full_nodes(t->right) + _number_of_full_nodes(t->left));
        }
    }

    void _remove_leaves(gv_node *&node, set<T> *&s) {
        if (node == nullptr) {
            return;
        }

        /*If both subtrees are null, node is leaf and is removed*/
        if (node->right == nullptr && node->left == nullptr) {
            s->insert(node->data);
            remove(node->data, node);
            return;
        }
        _remove_leaves(node->right, s);
        _remove_leaves(node->left, s);
    }

    /*Returns all value in a tree that are within the range of k1-k2*/
    void _get_range(vector<T> *&vec, gv_node *t, const int k1, const int k2) const {
        if (t == nullptr) {
            return;
        }
        if (t->data >= k1 && t->data <= k2) {

            /*Inorder search, parent data is only inserted into vector after left subtree is searched */
            _get_range(vec, t->left, k1, k2);
            vec->push_back(t->data);
            _get_range(vec, t->right, k1, k2);
        }

        /*If node data is < k1 or > k2, search only the right and left subtrees respectively */
        else if (t->data < k1) {
            _get_range(vec, t->right, k1, k2);
        }
        else if (t->data > k2) {
            _get_range(vec, t->left, k1, k2);
        }
    }

};

/*---------------------------------------------------*
 *   The gv_node structure (Page 126, lines 22-30)   *
 *---------------------------------------------------*/
template<typename T>
struct BinarySearchTree<T>::gv_node {
    T data;
    gv_node *left;
    gv_node *right;
};
#endif
