//Simple Dynamic Array-based List
//implements the Linked List using backing array

//maybe change all the ints to size_t later?

//ARRAY DOES NOT INIT TO RIGHT SIZE CURRENTLY
//NEW ARRAY ALLOCATION NOT YET IMPLEMENTED

#ifndef _SDAL_H_
#define _SDAL_H_
#include <stdexcept>
#include "LinkedList.h"

namespace cop3530 {

template <typename E>
class SDAL : public LinkedList<E> {
	public:
	SDAL(size_t size);
	~SDAL() override;
	
	Node<E> * new_node(E element) override;
	void push_back(E element) override;
	void push_front(E element) override;
	void insert(E element, int pos); 
	void replace(E element, int pos);
	E remove(int pos);
	E pop_back() override;
	E pop_front() override;
	E item_at(int pos);
	E peek_back() override;
	E peek_front() override;
	bool is_empty() override; 
	bool is_full() override;
	size_t length() override;
	void clear() override;
	
	//bool contains(E element, void (*equals_function)(E)) override;	
	void print(std::ostream& stream) override;
	E* const contents() override;
	
	
	private: 
	E *array;
	size_t array_size = 0;
	//in this implementation, tail is an index
	//head pointer will always be array[0]
	size_t head = 0;
	size_t tail = -1;
};

//---constructors and destructors
template <typename E>
SDAL<E>::SDAL(size_t size) {
	//set private size variable
	//init array of that size
	//default to 50 if none given
	array_size = size;
	array = new E[size];
	/*
	if (size == 0) {
		array = new E[50];
	}
	else {
		array = new E[size];
	}
	*/
}

template <typename E>
SDAL<E>::~SDAL() {
	delete array;
}

//---new_node()
//does nothing for SDAL, just there because of inheritance, leftover from Linkedlist.h
template <typename E>
Node<E> * SDAL<E>::new_node(E element) {
	class Node<E> *temp_Node;
	temp_Node = new Node<E>;
	return temp_Node;
}

//---push_back()
template <typename E>
void SDAL<E>::push_back(E element) {
	//check for errors
	tail++;
	array[tail] = element;
	
	std::cout<<"Pushed to back!"<<std::endl;
}

//---push_front()
template <typename E>
void SDAL<E>::push_front(E element) {
	//check for errors
	size_t size = this->length();
	for (int i = size;  i > 0; i--) {
		array[i] = array[i - 1];
	}
	tail++;
	array[head] = element;
	std::cout<<"Pushed to front!"<<std::endl;
}

//---insert()
//check if pos will be invalid
template <typename E>
void SDAL<E>::insert(E element, int pos) {
	//check if invalid index
	size_t size = this->length();
	for (int i = size; i > pos; i--) {
		array[i] = array[i - 1];
	}
	tail++;
	array[pos] = element;
	std::cout<<"Inserted!"<<std::endl;
}

//---replace()
template <typename E>
void SDAL<E>::replace(E element, int pos) {
	//check if invalid index
	array[pos] = element;
	std::cout<<"Replaced!"<<std::endl;
}

//---remove()
template <typename E>
E SDAL<E>::remove(int pos) {
	size_t size = this->length();
	E value = array[pos];
	for (int i = pos; i < size; i++) {
		array[i] = array[i + 1];
	}
	tail--;
	return value;
}

//---pop_back()
template <typename E>
E SDAL<E>::pop_back() {
	E value;
	value = array[tail];
	tail--;
	return value;
}

//---pop_front()
template <typename E>
E SDAL<E>::pop_front() {
	size_t size = this->length();
	E value = array[head];
	for (int i = 0; i < size; i++) {
		array[i] = array[i + 1];
	}
	tail--;
	return value;
}

//---item_at()
template <typename E>
E SDAL<E>::item_at(int pos) {
	E value = array[pos];
	return value;
}

//---peek_back()
template <typename E>
E SDAL<E>::peek_back() {
	E value = array[tail];
	return value;
}

//---peek_front()
template <typename E>
E SDAL<E>::peek_front() {
	E value = array[head];
	return value;
}

//---is_empty()
template <typename E>
bool SDAL<E>::is_empty() {
	if (array[head] == 0 && tail == 0) {
		std::cout<<"Empty!"<<std::endl;
		return true;
	}
	else {
		std::cout<<"Not Empty!"<<std::endl;
		return false;
	}
}

//---is_full()
template <typename E>
bool SDAL<E>::is_full() {
	size_t size = this->length();
	if (size == array_size) {
		std::cout<<"Array is full!"<<std::endl;
		return true;
	}
	else {
		std::cout<<"Array is not full!"<<std::endl;
		return false;
	}
}

//---length()
template <typename E>
size_t SDAL<E>::length() {
	size_t length = tail + 1;
	return length;
}

//---clear()
template <typename E>
void SDAL<E>::clear() {
	array = {0};
}

//---contains()
//figure out what equals_fuctions is supposed to be
/*
template <typename E>
bool SDAL<E>::contains(E element, void (*equals_function)(E)) {
	
}
*/

//---print()
template <typename E>
void SDAL<E>::print(std::ostream& stream) {
	if (tail == -1) {
		std::cout<<"List is empty!"<<std::endl;
		return;
	}
	std::cout<<"[ ";
	for (int i = 0; i <= tail; i++) {
		std::cout<<array[i]<<", ";
	}
	std::cout<<"]"<<std::endl;
}

//---contents()
template <typename E>
E* const SDAL<E>::contents() {
	E * copied_Array = new E[this->tail + 1];
	for (int i = 0; i < tail; i++) {
		copied_Array[i] = array[i];
	}
	
	//print array for testing purposes
	/*
	for (i = 0; i < size; i++) {
		std::cout<<array[i]<<" ";
	}
	*/
	
	return copied_Array;
}

}
#endif
