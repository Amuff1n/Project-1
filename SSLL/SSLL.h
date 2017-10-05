//Simple Singly-Linked List
//implements the Linked List using a simple list

//maybe change all the ints to size_t later?

#ifndef _SSLL_H_
#define _SSLL_H_
#include <stdexcept>
#include "LinkedList.h"

namespace cop3530 {

template <typename E>
class SSLL : public LinkedList<E> {
	public:
	SSLL();
	~SSLL() override;
	
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
	void clear() override;
	
	//bool contains(E element, void (*equals_function)(E)) override;	
	void print(std::ostream& stream) override;
	E* const contents() override;
	
	//type aliases
	using size_t = std::size_t;
	using value_type = E;
	
	private: 
	Node<E> * head = 0;
	Node<E> * tail = 0;
	
	public:
	//iterator stuff
	template <typename DataT>
	class SSLL_Iter {
		public:
		//type aliases required for C++ iterator compatibility
		using size_t = std::size_t;
		using value_type = DataT;
		using reference = DataT&;
		using pointer = DataT*;
		using difference_type = std::ptrdiff_t;
		using iterator_category = std::forward_iterator_tag;
		
		//type aliases for prettier(!) code
		using self_type = SSLL_Iter;
		using self_reference = SSLL_Iter&;
		
		private:
		Node<E> * here;
		
		public:
		//constructor
		explicit SSLL_Iter(Node<E> * start = nullptr) : here(start) {}
		
		//operations
		reference operator*() const {
			return here->data;
		}
		pointer operator->() const {
			return &(operator*());
		}
		
		//copy and assignment
		SSLL_Iter( const SSLL_Iter& src) : here (src.here) {}
		self_reference operator = ( SSLL_Iter<DataT> const& src) {
			if (this == &src) {
				return (*this);
			}
			here = src.here;
			return *this;
		}
		//preincrement
		self_reference operator++() {
			if (here) {
				here = here->next;
			}
			return *this;
		} 
		//postincrement
		self_type operator++(int) {
			self_type tmp = *this;
			++(*this);
			return tmp;
		} 
		
		bool operator == ( SSLL_Iter<DataT> const& rhs) const {
			return (here == rhs.here);
		}
		bool operator != ( SSLL_Iter<DataT> const& rhs) const {
			return (here != rhs.here);
		}
	}; //end of iter
	
	public: 
	//iter type aliases after iter code
	using iterator = SSLL_Iter<E>;
	using const_iterator = SSLL_Iter<E const>;
	
	//methods to create iters
	iterator begin() {
		iterator i(head);
		return i;
	}
	iterator end() { 
		iterator i(tail->next);
		return i;
	}
	
	const_iterator begin() const {
		const_iterator i(head);
		return i;
	}
	const_iterator end() const {
		const_iterator i(tail->next);
		return i;
	}
};

//---constructors and destructors
template <typename E>
SSLL<E>::SSLL() {
	//nothing to really init
	//set head and tail pointers to 0
	this->head = 0;
	this->tail = 0;
}

template <typename E>
SSLL<E>::~SSLL() {
	//node's should delete (deallocate) themselves in SSLL
	//so the deconstructor shouldn't have to do anything
}

//---new_node()
template <typename E>
Node<E> * SSLL<E>::new_node(E element) {
	class Node<E> *temp_Node;
	temp_Node = new Node<E>;
	//catch error here if memory is not available when creating temp_Node
	temp_Node->data = element;
	temp_Node->next = 0;
	return temp_Node;
}

//---push_back()
template <typename E>
void SSLL<E>::push_back(E element) {
	//check for errors
	class Node<E> *temp_Node, *temp;
	temp_Node = this->new_node(element);
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
void SSLL<E>::push_front(E element) {
	//check for errors
	class Node<E> *temp_Node, *temp;
	temp_Node = this->new_node(element);
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
void SSLL<E>::insert(E element, int pos) {
	class Node<E> *temp,*prev, *insert_node;
	temp = head;
	if (head == 0) {
		std::cout<<"List is empty!"<<std::endl;
		return;
	}
	else if (pos == 1) {
		temp = head;
		insert_node = this->new_node(element);
		head = insert_node;
		insert_node->next = temp;
		return;
	}
	else {
		for (int i = 0; i < pos - 1; i++) {
			prev = temp;
			temp = temp->next;
		}
		insert_node = this->new_node(element);
		insert_node->next = temp;
		prev->next = insert_node;
	}
}

//---replace()
template <typename E>
void SSLL<E>::replace(E element, int pos) {
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
E SSLL<E>::remove(int pos) {
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
E SSLL<E>::pop_back() {
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
E SSLL<E>::pop_front() {
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
E SSLL<E>::item_at(int pos) {
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
E SSLL<E>::peek_back() {
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
E SSLL<E>::peek_front() {
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
bool SSLL<E>::is_empty() {
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
bool SSLL<E>::is_full() {
	std::cout<<"If this is full you have a problem"<<std::endl;
	return false;
}

//---length()
template <typename E>
size_t SSLL<E>::length() {
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
void SSLL<E>::clear() {
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
bool SSLL<E>::contains(E element, void (*equals_function)(E)) {
	
}
*/

//---print()
template <typename E>
void SSLL<E>::print(std::ostream& stream) {
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
E* const SSLL<E>::contents() {
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
