//Pool-using Singly-Linked List
//implements the Linked List using a simple list
//instead of ourright deleting nodes, they are added to a free 'pool'
//this saves on allocating and deallocating memory

//maybe change all the ints to size_t later?

#ifndef _PSLL_H_
#define _PSLL_H_
#include <stdexcept>
#include "LinkedList.h"

namespace cop3530 {

template <typename E>
class PSLL : public LinkedList<E> {
	public:
	PSLL();
	~PSLL() override;
	
	Node<E> * new_node(E element) override;
	void push_back(E element) override;
	void push_front(E element) override;
	void insert(E element, int pos);// override; //does not override? might be size_t == int 
	void replace(E element, int pos);// override; //does not override?
	E remove(int pos);// override; //does not override?
	E pop_back() override;
	E pop_front() override;
	E item_at(int pos);// override; //does not override?
	E peek_back() override;
	E peek_front() override;
	bool is_empty() override; 
	bool is_full() override;
	size_t length() override;
	size_t pool_length();
 	void clear() override;
	
	//bool contains(E element, void (*equals_function)(E)) override;	
	void print(std::ostream& stream) override;
	E* const contents() override;
	
	
	private: 
	Node<E> * head = 0;
	Node<E> * tail = 0;
	//pointer to the beginning of the pool list
	Node<E> * head_free = 0;
};

//---constructors and destructors
template <typename E>
PSLL<E>::PSLL() {
	//nothing to really init
	//set head and tail pointers to 0
	this->head = 0;
	this->tail = 0;
	this->head_free = 0;
}

template <typename E>
PSLL<E>::~PSLL() {
	//deconstructor should just delete the free pool
}

//---new_node()
template <typename E>
Node<E> * PSLL<E>::new_node(E element) {
	class Node<E> *temp_Node, *temp, *prev;
	//check if list is >= 100 nodes before creating a new one
	//also check if pool > half list length
	//if so, delete half the nodes
	size_t list_length = this->length();
	size_t pool_length = this->pool_length();
	if (list_length >= 100 || pool_length > (list_length/2)) {
		temp = head_free;
		for (int i = 0; i < (list_length/2); i++) {
			prev = temp;
			temp = temp->next;
			delete prev;
		}
	}
	temp_Node = new Node<E>;
	//catch error here if memory is not available when creating temp_Node
	temp_Node->data = element;
	temp_Node->next = 0;
	return temp_Node;
}

//---push_back()
template <typename E>
void PSLL<E>::push_back(E element) {
	//check for errors
	class Node<E> *temp_Node, *temp;
	if (head_free == 0) {
		temp_Node = this->new_node(element);
	}
	else {
		temp_Node = head_free;
		head_free = head_free->next;
	}
	if (tail == 0) {
		head = temp_Node;
		tail = temp_Node;
		tail->next = 0;
	}
	else {
		temp = tail;
		temp->next = temp_Node;
		tail = temp_Node;
	}
	
	std::cout<<"Pushed to back!"<<std::endl;
}

//---push_front()
template <typename E>
void PSLL<E>::push_front(E element) {
	//check for errors
	class Node<E> *temp_Node, *temp;
	if (head_free == 0) {
		temp_Node = this->new_node(element);
	}
	else {
		temp_Node = head_free;
		head_free = head_free->next;
	}
	if (head == 0) {
		head = temp_Node;
		tail = temp_Node;
		head->next = 0;
	}
	else {
		temp = head;
		head = temp_Node;
		head->next = temp;
	}
	std::cout<<"Pushed to front!"<<std::endl;
}

//---insert()
//check if pos will be invalid
template <typename E>
void PSLL<E>::insert(E element, int pos) {
	class Node<E> *temp,*prev, *insert_node;
	temp = head;
	if (head == 0) {
		std::cout<<"List is empty!"<<std::endl;
		return;
	}
	else if (pos == 1) {
		temp = head;
		if (head_free == 0) {
			insert_node = this->new_node(element);
		}
		else {
			insert_node = head_free;
			head_free = head_free->next;
		}
		head = insert_node;
		insert_node->next = temp;
		return;
	}
	else {
		for (int i = 0; i < pos - 1; i++) {
			prev = temp;
			temp = temp->next;
		}
		if (head_free == 0) {
			insert_node = this->new_node(element);
		}
		else {
			insert_node = head_free;
			head_free = head_free->next;
		}
		insert_node->next = temp;
		prev->next = insert_node;
	}
}

//---replace()
template <typename E>
void PSLL<E>::replace(E element, int pos) {
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
E PSLL<E>::remove(int pos) {
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
	temp->next = head_free;
	head_free = temp;
	return value;
}

//---pop_back()
template <typename E>
E PSLL<E>::pop_back() {
	E value;
	if (head == 0) {
		std::cout<<"List is empty!"<<std::endl;
		return 0;
	}
	//special case if list is size 1
	else if (head == tail){
		value = head->data;
		tail->next = head_free;
		head_free = tail;
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
		temp->next = head_free;
		head_free = temp;
		tail->next = 0;
		return value;
	}
}
//---pop_front()
template <typename E>
E PSLL<E>::pop_front() {
	E value;
	class Node<E> *temp_Node;
	if (head == 0) {
		std::cout<<"List is empty!"<<std::endl;
		return 0;
	}
	value = head->data;
	temp_Node = head;
	head = head->next;
	temp_Node->next = head_free;
	head_free = temp_Node;
	return value;
}

//---item_at()
template <typename E>
E PSLL<E>::item_at(int pos) {
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
E PSLL<E>::peek_back() {
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
E PSLL<E>::peek_front() {
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
bool PSLL<E>::is_empty() {
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
bool PSLL<E>::is_full() {
	std::cout<<"If this is full you have a problem"<<std::endl;
	return false;
}

//---length()
template <typename E>
size_t PSLL<E>::length() {
	class Node<E> *temp;
	temp = head;
	size_t length = 0;
	while (temp != 0) {
		temp = temp->next;
		length++;
	}
	return length;
}

//---pool_length()
template <typename E>
size_t PSLL<E>::pool_length() {
	class Node<E> *temp;
	temp = head_free;
	size_t length = 0;
	while (temp != 0) {
		temp = temp->next;
		length++;
	}
	return length;
}
//---clear()
template <typename E>
void PSLL<E>::clear() {
	if (head == 0) {
		std::cout<<"List is empty!"<<std::endl;
	}
	else {
		class Node<E> *temp,*prev;
		temp = head;
		//while loop to add nodes to pool list instead of delete
		while (temp != 0) {
			prev = temp;
			temp = temp->next;
			prev->next = head_free;
			head_free = prev;
		}
	head = tail = 0;
	}
}

//---contains()
//figure out what equals_fuctions is supposed to be
/*
template <typename E>
bool PSLL<E>::contains(E element, void (*equals_function)(E)) {
	
}
*/

//---print()
template <typename E>
void PSLL<E>::print(std::ostream& stream) {
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
E* const PSLL<E>::contents() {
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
