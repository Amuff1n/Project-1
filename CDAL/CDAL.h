//Chained Dynamic Array-based list
//combines the SSLL and the SDAL
//chain of nodes where each node has an array of data

//nodes have their own head and tail indices they keep track of
//pushing to front does not shift all elements to the right like in a normal array
//this is because a node's head index may not always be 0 and pushing front goes to the head index of a node
//if we hit the start of the array and want to push to front, we create a new node as the head node and push to that

//this will make pushing and popping front an O(1) operation instead of O(n) like the SDAL

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
	
	bool contains(E element);	
	void print(std::ostream& stream) override;
	E* const contents() override;
	
	
	private: 
	//head and tail pointers for the linked list
	Node<E> * head = 0;
	Node<E> * tail = 0;
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
	//TODO properly delete array when deleting node?
}

//---new_node()
template <typename E>
Node<E> * CDAL<E>::new_node(E element) {
	class Node<E> *temp_Node;
	//catch error here if memory is not available when creating temp_Node
	temp_Node = new Node<E>;
	temp_Node->array = new E[50];
	if (head == nullptr && tail == nullptr) {
		head = temp_Node;
		tail = temp_Node;
	}
	temp_Node->head_index = 0;
	temp_Node->tail_index = 0;
	
	return temp_Node;
}

//---push_back()
template <typename E>
void CDAL<E>::push_back(E element) {
	//check for errors
	class Node<E> *temp_Node;
	temp_Node = tail;
	//if our list has no nodes, make an initial one
	if (temp_Node == 0) {
		temp_Node = this->new_node(element);
	}
	//if current node array is full, make new Node at tail
	else if (temp_Node->tail_index == 49) {
		temp_Node = this->new_node(element);
		tail->next = temp_Node;
		tail = temp_Node;
	}
	else {
		tail->tail_index++;
	}
	
	tail->array[tail->tail_index] = element;
	std::cout<<"Pushed to back!"<<std::endl;
}

//---push_front()
template <typename E>
void CDAL<E>::push_front(E element) {
	class Node<E> *temp_Node;
	temp_Node = head;
	if (temp_Node == nullptr) {
		temp_Node = this->new_node(element);
	} 
	else if (temp_Node->head_index < 0) {
		head->array[head->head_index] = element;
		temp_Node = this->new_node(element);
		temp_Node->next = head;
		temp_Node->head_index = 49;
		temp_Node->tail_index = 49;
		head = temp_Node;
	}
	else {
		head->head_index--;
		head->array[head->head_index] = element;
	}
	
	std::cout<<"Pushed to front!"<<std::endl;
}

//TODO FIGURE OUT A SOLUTION FOR THIS 
//i don't want to have to manually move everything up but I just might

//---insert()
//check if pos will be invalid
template <typename E>
void CDAL<E>::insert(E element, int pos) {
	/*
	class Node<E> *temp_Node;
	temp_Node = head;
	if (head == 0) {
		std::cout<<"List is empty!"<<std::endl;
		return;
	}
	size_t size = this->length();
	if (pos >size) {
		std::cout<<"Position invalid"<<std::endl;
		return;
	}
	std::cout<<"Inserted!"<<std::endl;
	*/
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
	size_t size = this->length();
	if (pos > size || pos <= 0) {
		std::cout<<"Position Invalid"<<std::endl;
	}
	
	std::cout<<"Replaced!"<<std::endl;
}

//TODO fix this, this is currently just copied from SDAL
//---remove()
template <typename E>
E CDAL<E>::remove(int pos) {
	/*
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
	*/
}

//---pop_back()
template <typename E>
E CDAL<E>::pop_back() {
	E value;
	if (head == 0 || head->tail_index == -1) {
		std::cout<<"List is empty!"<<std::endl;
		return 0;
	}
	value = tail->array[tail->tail_index];
	tail->tail_index--;
	
	return value;
}
//---pop_front()
template <typename E>
E CDAL<E>::pop_front() {
	E value;
	class Node<E> *temp_Node;
	if (head == 0 || head->tail_index == -1) {
		std::cout<<"List is empty!"<<std::endl;
		return 0;
	}
	value = head->array[head->head_index];
	head->head_index++;
	
	return value;
}

//TODO fix this, currently just copied from SDAL
//---item_at()
template <typename E>
E CDAL<E>::item_at(int pos) {
	/*
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
	*/
}

//---peek_back()
template <typename E>
E CDAL<E>::peek_back() {
	E value;
	if (tail == 0 || tail->tail_index == -1) {
		std::cout<<"List is empty!"<<std::endl;
		return 0;
	}
	else {
	value = tail->array[tail->tail_index];
	return value;
	}
}

//---peek_front()
template <typename E>
E CDAL<E>::peek_front() {
	E value;
	if (head == 0 || head->tail_index == -1) {
		std::cout<<"List is empty!"<<std::endl;
		return 0;
	}
	else {
	value = head->array[head->head_index];
	return value;
	}
}

//---is_empty()
template <typename E>
bool CDAL<E>::is_empty() {
	if (head == 0 || head->tail_index == -1) {
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
		length = length + (temp->tail_index - temp->head_index);
		temp = temp->next;
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
//TODO figure out what equals function should be and how the hell it works
template <typename E>
bool CDAL<E>::contains(E element) {
	
}


//---print()
template <typename E>
void CDAL<E>::print(std::ostream& stream) {
	class Node<E> *temp;
	temp = head;
	if (head == 0 || temp->tail_index == -1) {
		std::cout <<"<empty list>"<< std::endl;
	}
	
	std::cout<<"[ ";
	while (temp != 0) {
		for (int i = temp->head_index; i < temp->tail_index; i++) {
			std::cout<<temp->array[i]<<", ";
		}
		temp=temp->next;
	}
	std::cout<<"]"<<std::endl;
}

//---contents()
template <typename E>
E* const CDAL<E>::contents() {
	class Node<E> *temp;
	temp = head;
	if (head == 0 || temp->tail_index == -1) {
		std::cout<<"List is empty!"<<std::endl;
	}
	
	int size = this->length();
	E *copy_array = new E[size];
	int j = 0;
	
	while (temp != 0) {
		for (int i = temp->head_index; i < temp->tail_index; i++) {
			copy_array[j] = temp->array[i];
			j++;
		}
		temp=temp->next;
	}
	
	//print array for testing purposes
	/*
	for (i = 0; i < size; i++) {
		std::cout<<array[i]<<" ";
	}
	*/
	
	return copy_array;
}

}
#endif
