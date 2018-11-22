#include <memory>
#include <algorithm>
#include <iostream>

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


std::shared_ptr<Node> rotateToRight(std::shared_ptr<Node> top){
	std::shared_ptr<Node> l = top->left;
	std::shared_ptr<Node> lr = l->right;
	
	//rotate
	l->right = top;
	top->left = lr;
	
	//update height
	top->height = std::max(top->left->height, y->right->height) +1;
	l->height = std::max(l->left->height, l->right->height)+1;
	
	return l;
}

std::shared_ptr<Node> rotateToLeft(std::shared_ptr<Node> top){
	std::shared_ptr<Node> r = top->right;
	std::shared_ptr<Node> rl = l->left;
	
	//rotate
	r->left = top;
	top->right = rl;
	
	//update height
	top->height = std::max(top->left->height, y->right->height) +1;
	r->height = std::max(r->left->height, r->right->height)+1;
	
	return l;
}


std::shared_ptr<Node> insert(std::shared_ptr<Node> tree, int value){
	
}
