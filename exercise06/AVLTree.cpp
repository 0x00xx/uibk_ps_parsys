#include <memory>

class Node{
	public:
	int value;
	std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;
    Node(int value) : value(value){
		
    }
    ~Node(){
         
    }
}

std::shared_ptr<Node> insert(std::shared_ptr<Node> tree, int value){
	
}
