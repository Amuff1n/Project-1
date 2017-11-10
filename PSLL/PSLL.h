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
	~PSLL() override; //destructor
	PSLL(const PSLL& other); //copy constructor
	PSLL<E>& operator= (const PSLL& other); //copy-assignment operator
	PSLL(PSLL&& other); //move constructor
	PSLL<E>& operator= (PSLL&& other); //move-assignment operator
	
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
	size_t pool_length();
 	void clear() override;
	
	bool contains(E element, bool (*equals_function)(E,E));	
	void print(std::ostream& stream) override;
	E* const contents() override;
	
	//type aliases
	//using size_t = std::size_t;
	using value_type = E;
	
	private: 
	Node<E> * head = nullptr;
	Node<E> * tail = nullptr;
	//pointer to the beginning of the pool list
	Node<E> * head_free = nullptr;
	
	public:
	//iterator stuff
	template <typename DataT>
	class PSLL_Iter {
		public:
		//type aliases required for C++ iterator compatibility
		//using size_t = std::size_t;
		using value_type = DataT;
		using reference = DataT&;
		using pointer = DataT*;
		using difference_type = std::ptrdiff_t;
		using iterator_category = std::forward_iterator_tag;
		
		//type aliases for prettier(!) code
		using self_type = PSLL_Iter;
		using self_reference = PSLL_Iter&;
		
		private:
		Node<E> * here;
		
		public:
		//constructor
		explicit PSLL_Iter(Node<E> * start = nullptr) : here(start) {}
		
		//operations
		reference operator*() const {
			return here->data;
		}
		pointer operator->() const {
			return &(operator*());
		}
		
		//copy and assignment
		PSLL_Iter( const PSLL_Iter& src) : here (src.here) {}
		self_reference operator = ( PSLL_Iter<DataT> const& src) {
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
		
		bool operator == ( PSLL_Iter<DataT> const& rhs) const {
			return (here == rhs.here);
		}
		bool operator != ( PSLL_Iter<DataT> const& rhs) const {
			return (here != rhs.here);
		}
	}; //end of iter
	
	public: 
	//iter type aliases after iter code
	using iterator = PSLL_Iter<E>;
	using const_iterator = PSLL_Iter<E const>;
	
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
PSLL<E>::PSLL() {
	//nothing to really init
	//set head and tail pointers to 0
	this->head = nullptr;
	this->tail = nullptr;
	this->head_free = nullptr;
}

template <typename E>
PSLL<E>::~PSLL() {
	//add all nodes to pool, delete pool
	this->clear();
	class Node<E> *temp,*prev;
	temp = head_free;
	while (temp != nullptr) {
		prev = temp;
		temp = temp->next;
		delete prev;
	}
}

//---copy constructor
template <typename E>
PSLL<E>::PSLL(const PSLL& other) {
	//copy list from other
	class Node<E> *temp;
	temp = other.head;
	while (temp) {
		this->push_back(temp->data);
		temp = temp->next;
	}
	head_free = nullptr; //empty free pool
}

//---copy-assignment
//may have exception problems if other throws exceptions
//could probably save some complexity by copying values
//but i'm lazy so this just deletes the chain and recreates the other one
template <typename E>
PSLL<E>& PSLL<E>::operator=(const PSLL& other) {
	if (this != &other) {
		//delete current list, instead of adding it to pool
		//want to keep the current pool
		class Node<E> *temp,*prev;
		temp = head;
		while (temp != nullptr) {
			prev = temp;
			temp = temp->next;
			delete prev;
		}
		head = tail = nullptr;
		
		//copy list from other
		temp = other.head;
		while (temp) {
			this->push_back(temp->data);
			temp = temp->next;
		}
	}
	return *this;
}

//---move constructor
//assuming we leave each PSLL's pool alone for move
template <typename E>
PSLL<E>::PSLL(PSLL&& other) {
	head = other->head;
	tail = other->tail;
	//set other to default values to avoid being automatically destroyed
	other->head = nullptr;
	other->tail = nullptr;
}

//---move-assignment
//assuming we leave each PSLL's pool alone for move
template <typename E>
PSLL<E>& PSLL<E>::operator=(PSLL&& other) {
	//make sure we aren't referencing ourself
	if (this != &other) {
		//free existing data
		class Node<E> *temp,*prev;
		temp = head;
		while (temp != nullptr) {
			prev = temp;
			temp = temp->next;
			delete prev;
		}
		head = tail = nullptr;
		
		head = other->head;
		tail = other->tail;
		
		//set other to default values to avoid being automatically destroyed
		other->head = nullptr;
		other->tail = nullptr;
	}
	return *this;
}

//---new_node()
template <typename E>
Node<E> * PSLL<E>::new_node(E element) {
	class Node<E> *temp_Node, *temp, *prev;
	//check if list is >= 100 nodes before creating a new one
	//also check if pool > half list length
	//if so, delete half the nodes
	//'new' will throw exception if no memeory left
	size_t list_length = this->length();
	size_t pool_length = this->pool_length();
	if (list_length >= 100 && pool_length > (list_length/2)) {
		temp = head_free;
		for (int i = 0; i < (list_length/2); i++) {
			prev = temp;
			temp = temp->next;
			delete prev;
		}
	}
	temp_Node = new Node<E>;
	temp_Node->data = element;
	temp_Node->next = nullptr;
	return temp_Node;
}

//---push_back()
template <typename E>
void PSLL<E>::push_back(E element) {
	//check for errors
	class Node<E> *temp_Node, *temp;
	if (head_free == nullptr) {
		temp_Node = this->new_node(element);
	}
	else {
		temp_Node = head_free;
		head_free = head_free->next;
		temp_Node->data = element;
		temp_Node->next = nullptr;
	}
	if (tail == nullptr) {
		head = temp_Node;
		tail = temp_Node;
		tail->next = nullptr;
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
	if (head_free == nullptr) {
		temp_Node = this->new_node(element);
	}
	else {
		temp_Node = head_free;
		head_free = head_free->next;
		temp_Node->data = element;
		temp_Node->next = nullptr;
	}
	if (head == nullptr) {
		head = temp_Node;
		tail = temp_Node;
		head->next = nullptr;
	}
	else {
		temp = head;
		head = temp_Node;
		head->next = temp;
	}
	std::cout<<"Pushed to front!"<<std::endl;
}

//---insert()
template <typename E>
void PSLL<E>::insert(E element, int pos) {
	class Node<E> *temp,*prev, *insert_node;
	temp = head;
	if (head == nullptr) {
		std::cout<<"List is empty!"<<std::endl;
		return;
	}
	int size = this->length();
	if (pos > size || pos <= 0) {
		std::cout<<"Invalid position"<<std::endl;
		return;
	}
	else if (pos == 1) {
		temp = head;
		if (head_free == nullptr) {
			insert_node = this->new_node(element);
		}
		else {
			insert_node = head_free;
			head_free = head_free->next;
			insert_node->data = element;
			insert_node->next = nullptr;
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
		if (head_free == nullptr) {
			insert_node = this->new_node(element);
		}
		else {
			insert_node = head_free;
			head_free = head_free->next;
			insert_node->data = element;
			insert_node->next = nullptr;
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
	if (head == nullptr) {
		std::cout<<"List is empty!"<<std::endl;
		return;
	}
	int size = this->length();
	if (pos > size || pos <= 0) {
		std::cout<<"Invalid position"<<std::endl;
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
	if (head == nullptr) {
		std::cout<<"List is empty!"<<std::endl;
		return 0;
	}
	int size = this->length();
	if (pos > size || pos <= 0) {
		std::cout<<"Invalid position"<<std::endl;
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
	if (head == nullptr) {
		std::cout<<"List is empty!"<<std::endl;
		return 0;
	}
	//special case if list is size 1
	else if (head == tail){
		value = head->data;
		tail->next = head_free;
		head_free = tail;
		tail = head = nullptr;
		return value;
	}
	else {
		value = tail->data;
		//going to have to find new tail pointer, O(n), follow chain
		class Node<E> *temp;
		temp = head;
		while (temp->next->next != nullptr) {
			temp = temp->next;
		}
		tail = temp;
		temp->next = head_free;
		head_free = temp;
		tail->next = nullptr;
		return value;
	}
}
//---pop_front()
template <typename E>
E PSLL<E>::pop_front() {
	E value;
	class Node<E> *temp_Node;
	if (head == nullptr) {
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
	if (head == nullptr) {
		std::cout<<"List is empty!"<<std::endl;
		return 0;
	}
	int size = this->length();
	if (pos > size || pos <= 0) {
		std::cout<<"Invalid position"<<std::endl;
		return 0;
	}
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
	if (tail == nullptr) {
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
	if (head == nullptr) {
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
	if (head == nullptr) {
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
	while (temp != nullptr) {
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
	while (temp != nullptr) {
		temp = temp->next;
		length++;
	}
	return length;
}

//---clear()
template <typename E>
void PSLL<E>::clear() {
	if (head == nullptr) {
		std::cout<<"List is empty!"<<std::endl;
	}
	else {
		class Node<E> *temp,*prev;
		temp = head;
		//while loop to add nodes to pool list instead of delete
		while (temp != nullptr) {
			prev = temp;
			temp = temp->next;
			prev->next = head_free;
			head_free = prev;
		}
	head = tail = nullptr;
	}
}

//---contains()
template <typename E>
bool PSLL<E>::contains(E element, bool (*equals_function)(E,E)) {
	if (head == nullptr) {
		std::cout<<"List is empty!"<<std::endl;
		return false;
	}
	else {
		class Node<E> *temp;
		temp = head;
		while (temp != nullptr) {
			if (equals_function(element,temp->data)) {
				std::cout<<element<<" exists in list!"<<std::endl;
				return true;
			}
			temp = temp->next;
		}
		std::cout<<element<<" is not in list!"<<std::endl;
		return false;
	}
}

//---print()
template <typename E>
void PSLL<E>::print(std::ostream& stream) {
	if (head == nullptr) {
		std::cout <<"<empty list>"<< std::endl;
	}
	else {
		class Node<E> *temp;
		temp = head;
		std::cout<<"[ ";
		while (temp != nullptr) {
			std::cout<<temp->data<<", ";
			temp = temp->next;
		}
		std::cout<<"]"<<std::endl;
	}
}

//---contents()
template <typename E>
E* const PSLL<E>::contents() {
	if (head == nullptr) {
		std::cout<<"List is empty!"<<std::endl;
	}
	int size = this->length();
	E *array = new E[size];
	int i = 0;
	class Node<E> *temp;
	temp = head;
	while (temp != nullptr) {
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
