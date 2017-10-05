//Chained Dynamic Array-based list
//combines the SSLL and the SDAL
//chain of nodes where each node has an array of data

//MAYBE EACH NODE SHOULD HAVE ITS OWN HEAD INDEX AND TAIL INDEX

//maybe change all the ints to size_t later?

#ifndef _CDAL_H_
#define _CDAL_H_
#include <stdexcept>
#include "LinkedList.h"

namespace cop3530 {

template <typename E>
class CDAL : public LinkedList<E> {
	public:
	CDAL();
	~CDAL() override;
	
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
	//head and tail pointers for the linked list
	Node<E> * head = 0;
	Node<E> * tail = 0;
	//head and tail indices for the arrays in those nodes
	//head index will always be 0, but want to reduce magic numbers in code
	size_t head_index = 0;
	size_t tail_index = -1;
};

//create a new kind of node, array_node that extends node
template <typename E>
class array_node : public Node<E> {
	public:
	E * array = new E[50];
};

//---constructors and destructors
template <typename E>
CDAL<E>::CDAL() {
	//nothing to really init
	//set head and tail pointers to 0
	this->head = 0;
	this->tail = 0;
}

template <typename E>
CDAL<E>::~CDAL() {
	//node's should delete (deallocate) themselves in CDAL
	//so the deconstructor shouldn't have to do anything
}

//---new_node()
template <typename E>
Node<E> * CDAL<E>::new_node(E element) {
	class Node<E> *temp_Node;
	//catch error here if memory is not available when creating temp_Node
	temp_Node = new array_node<E>;
	if (head == 0) {
		head = temp_Node;
		tail = temp_Node;
	}
	else {
		tail->next = temp_Node;
		tail = temp_Node;
		temp_Node->next = 0;
	}
	tail_index = -1;
	return temp_Node;
}

//---push_back()
template <typename E>
void CDAL<E>::push_back(E element) {
	//check for errors
	class Node<E> *temp_Node;
	//if we have no nodes or current node full, create new node
	if (head == tail || tail_index == 50) {
		temp_Node = this->new_node(element);
	}
	else {
		temp_Node = tail;
	}
	tail_index++;
	temp_Node->array[tail] = element;
	
	std::cout<<"Pushed to back!"<<std::endl;
}

//---push_front()
template <typename E>
void CDAL<E>::push_front(E element) {
	//check if pushing front goes past array size
	//if so, have to create new node
	
	for (int i = tail;  i > 0; i--) {
		head->array[i] = array[i - 1];
	}
	tail++;
	head->array[head_index] = element;
	std::cout<<"Pushed to front!"<<std::endl;
}

//---insert()
//check if pos will be invalid
template <typename E>
void CDAL<E>::insert(E element, int pos) {
	class Node<E> *temp_Node;
	temp_Node = head;
	if (head == 0) {
		std::cout<<"List is empty!"<<std::endl;
		return;
	}
	while (div (pos,50)) {
		temp_Node = head->next;
	}
	//check if 
	for (int i = tail; i > 0; i--) {
		temp_Node->array[i] = array[i - 1];
	}
	tail++;
	temp_Node->array[pos] = element;
	std::cout<<"Inserted!"<<std::endl;
}

//---replace()
template <typename E>
void CDAL<E>::replace(E element, int pos) {
	class Node<E> *temp;
	temp = head;
	if (head == 0) {
		std::cout<<"List is empty!"<<std::endl;
		return;
	}
	for (int i = 0; i < pos - 1; i++) {
		//catch an invalid position here later
		temp = temp->next;
	}
	temp->data = element;
	std::cout<<"Replaced!"<<std::endl;
}

//---remove()
template <typename E>
E CDAL<E>::remove(int pos) {
	class Node<E> *temp, *prev;
	temp = head;
	if (head == 0) {
		std::cout<<"List is empty!"<<std::endl;
		return 0;
	}
	for (int i = 1; i < pos; i++) {
		prev = temp;
		temp = temp->next;
	}
	if (temp == head) {
		head = temp->next;
	}
	else if (temp == tail) {
		tail = prev;
	}
	E value = temp->data;
	prev->next = temp->next;
	delete temp;
	return value;
}

//---pop_back()
template <typename E>
E CDAL<E>::pop_back() {
	E value;
	if (head == 0) {
		std::cout<<"List is empty!"<<std::endl;
		return 0;
	}
	//special case if list is size 1
	else if (head == tail){
		value = head->data;
		delete tail;
		tail = head = 0;
		return value;
	}
	else {
		value = tail->data;
		//going to have to find new tail pointer, O(n), follow chain
		class Node<E> *temp;
		temp = head;
		while (temp->next->next != 0) {
			temp = temp->next;
		}
		tail = temp;
		delete tail->next;
		tail->next = 0;
		return value;
	}
}
//---pop_front()
template <typename E>
E CDAL<E>::pop_front() {
	E value;
	class Node<E> *temp_Node;
	if (head == 0) {
		std::cout<<"List is empty!"<<std::endl;
		return 0;
	}
	value = head->data;
	temp_Node = head;
	head = head->next;
	delete temp_Node;
	
	return value;
}

//---item_at()
template <typename E>
E CDAL<E>::item_at(int pos) {
	class Node<E> *temp, *prev;
	temp = head;
	if (head == 0) {
		std::cout<<"List is empty!"<<std::endl;
		return 0;
	}
	//handle invalid position
	for (int i = 1; i < pos; i++) {
		temp = temp->next;
	}
	E value = temp->data;
	return value;
}

//---peek_back()
template <typename E>
E CDAL<E>::peek_back() {
	E value;
	if (tail == 0) {
		std::cout<<"List is empty!"<<std::endl;
		return 0;
	}
	else {
	value = tail->data;
	return value;
	}
}

//---peek_front()
template <typename E>
E CDAL<E>::peek_front() {
	E value;
	if (head == 0) {
		std::cout<<"List is empty!"<<std::endl;
		return 0;
	}
	else {
	value = head->data;
	return value;
	}
}

//---is_empty()
template <typename E>
bool CDAL<E>::is_empty() {
	if (head == 0) {
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
bool CDAL<E>::is_full() {
	std::cout<<"If this is full you have a problem"<<std::endl;
	return false;
}

//---length()
template <typename E>
size_t CDAL<E>::length() {
	class Node<E> *temp;
	temp = head;
	size_t length = 0;
	while (temp != 0) {
		temp = temp->next;
		length++;
	}
	return length;
}

//---clear()
template <typename E>
void CDAL<E>::clear() {
	if (head == 0) {
		std::cout<<"List is empty!"<<std::endl;
	}
	else {
		class Node<E> *temp,*prev;
		temp = head;
		while (temp != 0) {
			prev = temp;
			temp = temp->next;
			delete prev;
		}
	head = tail = 0;
	}
}

//---contains()
//figure out what equals_fuctions is supposed to be
/*
template <typename E>
bool CDAL<E>::contains(E element, void (*equals_function)(E)) {
	
}
*/

//---print()
template <typename E>
void CDAL<E>::print(std::ostream& stream) {
	if (head == 0) {
		std::cout <<"<empty list>"<< std::endl;
	}
	else {
		class Node<E> *temp;
		temp = head;
		std::cout<<"[ ";
		while (temp != 0) {
			std::cout<<temp->data<<", ";
			temp = temp->next;
		}
		std::cout<<"]"<<std::endl;
	}
}

//---contents()
template <typename E>
E* const CDAL<E>::contents() {
	if (head == 0) {
		std::cout<<"List is empty!"<<std::endl;
	}
	int size = this->length();
	E *array = new E[size];
	int i = 0;
	class Node<E> *temp;
	temp = head;
	while (temp != 0) {
		array[i] = temp->data;
		temp = temp->next;
		i++;
	}
	
	//print array for testing purposes
	/*
	for (i = 0; i < size; i++) {
		std::cout<<array[i]<<" ";
	}
	*/
	
	return array;
}

}
#endif
