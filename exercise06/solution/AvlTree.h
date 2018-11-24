

#ifndef PARALLEL_AVLTREE_H
#define PARALLEL_AVLTREE_H


//#include "AvlNode.h"
template<typename> class AvlNode;
template <typename T>
class AvlTree
{
public:
    const AvlNode<T>* find(T value) const;
    unsigned int height() const;
    bool isValid() const;
    void insert(T value);
    void insertSeq(const T* arr, int len);
    void insertPar(const T* arr, int len);
    void setRoot(AvlNode<T>* newRoot);
    void printAllValues() const;
    int size() const;

private:
    AvlNode<T> *root = nullptr;
};

#endif //PARALLEL_AVLTREE_H
