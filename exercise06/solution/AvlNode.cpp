
#include <algorithm>
#include <iostream>
#include <iomanip>
#include "AvlNode.h"

template <typename T>
T AvlNode<T>::getValue() const
{
    return value;
}

template <typename T>
const AvlNode<T>* AvlNode<T>::find(const T value) const
{
    if (this->value == value)
        return this;

    if (value < this->value && left != nullptr)
        return left->find(value);

    if (value > this->value && right != nullptr)
        return right->find(value);

    return nullptr;
}

template <typename T>
unsigned int AvlNode<T>::height() const
{
    if (left && right)
        return std::max(left->height(), right->height()) + 1;
    if (left)
        return left->height() + 1;
    if (right)
        return right->height() + 1;
    return 1;
}

template <typename T>
void AvlNode<T>::setBalance() {

    if (!this->right && !this->left){
        this->balanceFactor = 0;
        return;
    }
    if (!this->right){
        this->balanceFactor = -this->left->height();
        return;
    }
    if (!this->left){
        this->balanceFactor = this->right->height();
        return;
    }
    this->balanceFactor = this->right->height() - this->left->height();
}



template <typename T>
bool AvlNode<T>::isValid() const
{
    return (((balanceFactor == 0 && ((!left && !right) || (left && right && left->height() == right->height()) )) ||
           (balanceFactor == 1 && right && ((!left && right->height() == 1) || right->height() == left->height() + 1)) ||
           (balanceFactor == -1 && left && ((!right && left->height() == 1) || right->height() == left->height() - 1)))
           && (!right||right->isValid()) && (!left||left->isValid()));
}

template <typename T>
void AvlNode<T>::rotateLeft() {
    AvlNode *tmpNode1 = this->right;
    AvlNode *tmpNode2 = tmpNode1->left?tmpNode1->left: nullptr;
    tmpNode1->left = this;
    this->right = tmpNode2;

    if(this->parent) {
        this->parent->replaceChild(this, tmpNode1);
        tmpNode1->parent = this->parent;
    }
    else {
        this->tree->setRoot(tmpNode1);
        tmpNode1->parent = nullptr;
    }

    if(tmpNode2) {
        tmpNode2->parent = this;
        tmpNode2->setBalance();
    }
    this->parent = tmpNode1;
    this->setBalance();
    tmpNode1->setBalance();

}


template <typename T>
void AvlNode<T>::rotateRight() {
    AvlNode *tmpNode1 = this->left;
    AvlNode *tmpNode2 = tmpNode1->right;

    tmpNode1->right = this;
    this->left = tmpNode2;


    if(this->parent) {
        this->parent->replaceChild(this, tmpNode1);
        tmpNode1->parent = this->parent;
    }
    else {
        this->tree->setRoot(tmpNode1);
        tmpNode1->parent = nullptr;
    }
    if(tmpNode2) {
        tmpNode2->parent = this;
        tmpNode2->setBalance();
    }
    this->parent = tmpNode1;
    this->setBalance();
    tmpNode1->setBalance();
}

template <typename T>
void AvlNode<T>::replaceChild(AvlNode<T> *oldChild, AvlNode<T> *newChild) {
    if (this->left != nullptr && this->left == oldChild)
        this->left = newChild;
    else if (this->right != nullptr && this->right == oldChild)
        this->right = newChild;
    else {
        std::cout << oldChild->value << " is no child of " << this->value << std::endl;
        throw std::runtime_error("replacing a child that doesn't exist!");
    }
}



template <typename T>
void AvlNode<T>::insert(T value)
{
    if (this->value == value){
        this->incCount(); 
        return;
    }
    //this->setBalance();

    if (this->value > value){
        if (!this->left) {
            this->left = new AvlNode<T>(value, this->tree, this);
        }
        else{
            #pragma omp task
            this->left->insert(value);
        }
    }
    else {
        if (!this->right) {
            this->right = new AvlNode<T>(value, this->tree, this);
        }
        else{
            #pragma omp task
            this->right->insert(value);
        }
    }

    #pragma omp taskwait

    this->setBalance();

    #pragma omp critical
    {
        if (this->balanceFactor > 1){
            if (value > this->right->value){
                this->rotateLeft();
            }
            else if (value < this->right->value) {
                this->right->rotateRight();
                this->rotateLeft();
            }
        }
        else if (this->balanceFactor < -1){
            if (value < this->left->value){
                this->rotateRight();
            }
            else if (value > this->left->value) {
                this->left->rotateLeft();
                this->rotateRight();
            }
        }
    }
}

template <typename T>
int AvlNode<T>::getCount() const
{
    return count;
}

template <typename T>
inline void AvlNode<T>::incCount(){
	#pragma omp atomic
	count++;
}

template <typename T>
int AvlNode<T>::size() const
{
    int tmp = count;
    if (right)
        tmp += right->size();
    if (left)
        tmp += left->size();
    return tmp;
}

// Explicit specialization to avoid linking issues
template class AvlNode<int>;
