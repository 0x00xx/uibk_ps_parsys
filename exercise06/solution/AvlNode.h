
#ifndef PARALLEL_AVLNODE_H
#define PARALLEL_AVLNODE_H

#include "AvlTree.h"
template <typename T>
class AvlNode
{
public:
    explicit AvlNode(T value, AvlTree<T> *tree) : value(value), tree(tree)
    {}

    // Getters
    T getValue() const;
    int getCount() const;

    // Algorithm
    const AvlNode<T>* find(T value) const;
    void insert(T value);
    unsigned int height() const;
    void incCount();
    void setBalance();

    // Verification
    bool isValid() const;
    void print(int indent=0) const;
    void print_info() const;
    int size() const;

private:
    AvlNode<T> *parent = nullptr;
    AvlNode<T> *left = nullptr;
    AvlNode<T> *right = nullptr;
    AvlTree<T> *tree = nullptr;
    T value;
    int balanceFactor = 0; // Update this on every call to insert()!
    int count = 1; //Count how many same elements we have

    explicit AvlNode(T value, AvlTree<T> *tree, AvlNode<T>* parent) : value(value), tree(tree), parent(parent)
    {}
    void rotateLeft();
    void rotateRight();
    void replaceChild(AvlNode<T> *oldChild,AvlNode<T> *newChild);
};


#endif //PARALLEL_AVLNODE_H
