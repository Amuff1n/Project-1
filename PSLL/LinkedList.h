//Linked List Class
//defines virtual linked lists used later

//should this be a struct?

#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_
#include "Node.h"
#include <stdio.h> //needed for size_t

namespace cop3530 {

template <typename E>
class LinkedList {
	public:
	// LinkedList's methods
	//what type is pos? should be overriden later by list type?
	//also apparently = 0 doesn't work for size_t, so those functions have {} instead
	virtual Node<E> * new_node(E) = 0;
	virtual void push_back(E element) = 0;
	virtual void push_front(E element) = 0;
	virtual void insert(E element, size_t pos) {};
	virtual void replace(E element, size_t pos) {};
	virtual E remove(size_t pos) {};
	virtual E pop_back() = 0;
	virtual E pop_front() = 0;
	virtual E item_at(size_t pos) {};
	virtual E peek_back() = 0;
	virtual E peek_front() = 0;
	virtual bool is_empty() = 0;
	virtual bool is_full() = 0;
	virtual size_t length() = 0;
	virtual void clear() = 0;
	
	virtual bool contains(E element, void (*equals_function)(E)) {};
	virtual void print(std::ostream& stream) = 0;
	virtual E* const contents() = 0;
	
	virtual ~LinkedList() {};
	
};

}
#endif