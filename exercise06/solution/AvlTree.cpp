
#include <iostream>
#include "AvlTree.h"
#include "AvlNode.h"
#include <omp.h>

template <typename T>
const AvlNode<T>* AvlTree<T>::find(const T value) const
{
    if (root)
        return root->find(value);

    return nullptr;
}

template <typename T>
unsigned int AvlTree<T>::height() const
{
    if (root)
        return root->height();
    return 0;
}

template <typename T>
bool AvlTree<T>::isValid() const
{
    if (root)
        return root->isValid();
    return true;
}

template <typename T>
void AvlTree<T>::insert(T value)
{
    if (root)
        root->insert(value);
    else
        root = new AvlNode<T>(value,this);
}

template <typename T>
void AvlTree<T>::printAllValues() const{
    std::cout << std::endl;
    if(root)
        root->print();
    std::cout << std::endl;
}

template <typename T>
void AvlTree<T>::setRoot(AvlNode<T> *newRoot) {
    this->root = newRoot;
}

template <typename T>
void AvlTree<T>::insertSeq(const T* arr,int len) {
    for (int i = 0; i < len; ++i) {
        AvlNode<T>* tmp = const_cast<AvlNode<T> *>(find(arr[i]));
        if(tmp)
            tmp->incCount();
        else
            this->insert(arr[i]);
    }

}


template <typename T>
void AvlTree<T>::insertPar(const T* arr,int len) {
    int threshold = len/8;
    this->insertSeq(arr,threshold);

   #pragma omp parallel
    {
        #pragma omp single
        {
            for (int i = threshold; i < len; ++i) {
                AvlNode<T> *tmp = const_cast<AvlNode<T> *>(find(arr[i]));
                if (tmp)
                    tmp->incCount();
                else {
                    #pragma omp task
                    {
                        this->insert(arr[i]);
                    }
                }
            }
        }
    }
}

template <typename T>
int AvlTree<T>::size() const
{
    if (this->root)
        return root->size();
    return 0;
}

// Explicit specialization to avoid linking issues
template class AvlTree<int>;
