//Linked List Class
//defines virtual linked lists used later

//should this be a struct?

#ifndef _LIST_H_
#define _LIST_H_
#include "Node.h"
#include <stdio.h> //needed for size_t

namespace cop3530 {

template <typename E>
class List {
	public:
	// List's methods
	//what type is pos? should be overriden later by list type?
	//i'll go with size_t for now
	
	virtual Node<E> * new_node(E) = 0;
	virtual void push_back(E element) = 0;
	virtual void push_front(E element) = 0;
	virtual void insert(E element, size_t pos) = 0;
	virtual void replace(E element, size_t pos) = 0;
	virtual E remove(size_t pos) = 0;
	virtual E pop_back() = 0;
	virtual E pop_front() = 0;
	virtual E item_at(size_t pos) = 0;
	virtual E peek_back() = 0;
	virtual E peek_front() = 0;
	virtual bool is_empty() = 0;
	virtual bool is_full() = 0;
	virtual size_t length() = 0;
	virtual void clear() = 0;
	
	virtual bool contains(E element, bool (*equals_function)(const E&,const E&)) = 0;
	virtual void print(std::ostream& stream) = 0;
	virtual E* const contents() = 0;
	
	virtual ~List() {};
	
};

}
#endif