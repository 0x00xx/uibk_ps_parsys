#include <iostream>
#include <memory>

#ifndef AVLTREE_H
#define AVLTREE_H

std::shared_ptr<Node> insert(std::shared_ptr<Node> tree, int value);
std::shared_ptr<Node> rotateToLeft(std::shared_ptr<Node> top);
std::shared_ptr<Node> rotateToRight(std::shared_ptr<Node> top);

class Node{
	public:
	int value;
	std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;
    int height;
    Node(int value) : value(value){
		left = NULL;
		right = NULL;
		height = 1;
    }
    ~Node(){
         
    }
};

#endif //AVLTREE_H
