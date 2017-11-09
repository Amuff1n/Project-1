//Node class
//defines the Nodes themselves

#ifndef _NODE_H_
#define _NODE_H_

namespace cop3530 {
	
template <typename E>
class Node {
	public:
	E  * array; //for CDAL, nodes will have a pointer to an array instead of just a value
	Node<E> * next;
	//Nodes will have their own head and tail indices but will not have their own operations
	//CDAL class does operations
	size_t head_index = 0;
	size_t tail_index = -1;
};

}
#endif