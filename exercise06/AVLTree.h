#include <iostream>
#include <memory>

#ifndef AVLTREE_H
#define AVLTREE_H

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
}

#endif //AVLTREE_H
