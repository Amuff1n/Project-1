//Node class
//defines the Nodes themselves

#ifndef _NODE_H_
#define _NODE_H_

namespace cop3530 {
	
template <typename E>
class Node {
	public:
	E data;
	Node<E> * next;
};

}
#endif