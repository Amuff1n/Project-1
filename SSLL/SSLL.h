//Simple Singly-Linked List
//implements the Linked List using a simple list

#ifndef _SSLL_H_
#define _SSLL_H_
#include <stdexcept>
#include "List.h"

namespace cop3530 {
	
template <typename E>
class Node {
	public:
	E data;
	Node<E> * next;
};

template <typename E>
class SSLL : public List<E> {
	public:
	explicit SSLL();
	~SSLL() override; //destructor
	SSLL(const SSLL& other); //copy constructor
	SSLL<E>& operator = (const SSLL& other); //copy-assignment operator
	SSLL(SSLL&& other); //move constructor
	SSLL<E>& operator = (SSLL&& other); //move-assignment operator
	
	
	Node<E> * new_node(E element);
	void push_back(E element) override;
	void push_front(E element) override;
	void insert(E element, size_t pos); 
	void replace(E element, size_t pos);
	E remove(size_t pos);
	E pop_back() override;
	E pop_front() override;
	E& item_at(size_t pos);
	const E& item_at(size_t pos) const;
	E& peek_back() override;
	const E& peek_back() const;
	E& peek_front() override;
	const E& peek_front() const;
	bool is_empty() override; 
	bool is_full() override;
	size_t length() override;
	const size_t length() const;
	void clear() override;
	
	bool contains(E element, bool (*equals_function)(const E&,const E&));	
	void print(std::ostream& stream) override;
	E* const contents() override;
	
	//type aliases
	//using size_t = std::size_t;
	using value_type = E;
	
	private: 
	Node<E> * head = nullptr;
	Node<E> * tail = nullptr;
	
	public:
	//iterator stuff
	template <typename DataT>
	class SSLL_Iter {
		public:
		//type aliases required for C++ iterator compatibility
		//using size_t = std::size_t;
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
	this->head = nullptr;
	this->tail = nullptr;
}

template <typename E>
SSLL<E>::~SSLL() {
	this->clear();
}

//---copy constructor
template <typename E>
SSLL<E>::SSLL(const SSLL& other) {
	class Node<E> *temp;
	temp = other.head;

	while (temp != nullptr) {
		this->push_back(temp->data);
		temp = temp->next;
	}
}

//---copy-assignment
//may have exception problems if 'other' throws exceptions
template <typename E>
SSLL<E>& SSLL<E>::operator=(const SSLL& other) {
	if (this == &other) {
		return (*this);
	}
	//clear self
	this->clear();
	
	class Node<E> *temp;
	temp = other.head;
	while (temp != nullptr) {
		this->push_back(temp->data);
		temp = temp->next;
	}
	return *this;
}

//---move constructor
template <typename E>
SSLL<E>::SSLL(SSLL&& other) {
	head = other.head;
	tail = other.tail;
	//set other to default values to avoid being automatically destroyed
	other.head = nullptr;
	other.tail = nullptr;
}

//---move-assignment
template <typename E>
SSLL<E>& SSLL<E>::operator = (SSLL&& other) {
	//make sure we aren't referencing ourself
	if (this == &other) {
		return *this;
	}
	//free existing data
	this->clear();
	head = other.head;
	tail = other.tail;
		
	//set other to default values to avoid being automatically destroyed
	other.head = nullptr;
	other.tail = nullptr;
	return *this;
}

//---new_node()
template <typename E>
Node<E> * SSLL<E>::new_node(E element) {
	//'new' will throw exception if out of memory
	class Node<E> *temp_Node;
	temp_Node = new Node<E>;
	temp_Node->data = element;
	temp_Node->next = nullptr;
	return temp_Node;
}

//---push_back()
template <typename E>
void SSLL<E>::push_back(E element) {
	class Node<E> *temp_Node, *temp;
	temp_Node = this->new_node(element);
	
	if (head == nullptr || tail == nullptr) {
		head = temp_Node;
		tail = temp_Node;
		tail->next = nullptr;
	}
	else {
		temp = tail;
		temp->next = temp_Node;
		tail = temp_Node;
	}
	
	//std::cout<<"Pushed to back!"<<std::endl;
}

//---push_front()
template <typename E>
void SSLL<E>::push_front(E element) {
	class Node<E> *temp_Node, *temp;
	temp_Node = this->new_node(element);
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
	//std::cout<<"Pushed to front!"<<std::endl;
}

//---insert()
template <typename E>
void SSLL<E>::insert(E element, size_t pos) {
	class Node<E> *temp,*prev, *insert_node;
	temp = head;
	int size = this->length();
	//check if pos will be invalid
	if (pos > size || pos < 0) {
		throw std::invalid_argument("Invalid position");
		return;
	}
	else if (size == 0) {
		insert_node = this->new_node(element);
		head = insert_node;
		tail = insert_node;
		insert_node->next = nullptr;
	}
	else if (pos == 0) {
		temp = head;
		insert_node = this->new_node(element);
		head = insert_node;
		insert_node->next = temp;
		return;
	}
	else {
		for (int i = 0; i < pos; i++) {
			prev = temp;
			temp = temp->next;
		}
		insert_node = this->new_node(element);
		insert_node->next = temp;
		if (insert_node->next == nullptr) {
			tail = insert_node;
		}
		prev->next = insert_node;
	}
}

//---replace()
template <typename E>
void SSLL<E>::replace(E element, size_t pos) {
	class Node<E> *temp;
	temp = head;
	if (head == nullptr) {
		throw std::runtime_error("List is empty!");
		return;
	}
	int size = this->length();
	if (pos > size || pos < 0) {
		throw std::invalid_argument("Invalid position");
		return;
	}
	for (int i = 0; i < pos; i++) {
		temp = temp->next;
	}
	temp->data = element;
	//std::cout<<"Replaced!"<<std::endl;
}

//---remove()
template <typename E>
E SSLL<E>::remove(size_t pos) {
	class Node<E> *temp, *prev;
	temp = head;
	if (head == nullptr) {
		throw std::runtime_error("List is empty!");
		return 0;
	}
	int size = this->length();
	if (pos > size || pos < 0) {
		throw std::invalid_argument("Invalid position");
		return 0;
	}
	//special case is pos == 0
	if (pos == 0) {
		head = head->next;
		E value = temp->data;
		delete temp;
		return value;
	}
	for (int i = 0; i < pos; i++) {
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
	if (head == nullptr) {
		throw std::runtime_error("List is empty!");
		return 0;
	}
	//special case if list is size 1
	else if (head == tail){
		value = head->data;
		delete tail;
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
		delete tail->next;
		tail->next = nullptr;
		return value;
	}
}
//---pop_front()
template <typename E>
E SSLL<E>::pop_front() {
	E value;
	class Node<E> *temp_Node;
	if (head == nullptr) {
		throw std::runtime_error("List is empty!");
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
E& SSLL<E>::item_at(size_t pos) {
	class Node<E> *temp, *prev;
	temp = head;
	if (head == nullptr) {
		throw std::runtime_error("List is empty!");
	}
	int size = this->length();
	if (pos > size || pos < 0) {
		throw std::invalid_argument("Invalid position");
	}
	for (int i = 0; i < pos; i++) {
		temp = temp->next;
	} 
	return temp->data;
}

//---item_at() const
template <typename E>
const E& SSLL<E>::item_at(size_t pos) const {
	class Node<E> *temp, *prev;
	temp = head;
	if (head == nullptr) {
		throw std::runtime_error("List is empty!");
	}
	int size = this->length();
	if (pos > size || pos < 0) {
		throw std::invalid_argument("Invalid position");
	}
	for (int i = 0; i < pos; i++) {
		temp = temp->next;
	} 
	return temp->data;
}

//---peek_back()
template <typename E>
E& SSLL<E>::peek_back() {
	if (tail == nullptr) {
		throw std::runtime_error("List is empty!");
	}
	else {
	return tail->data;
	}
}

//---peek_back() const
template <typename E>
const E& SSLL<E>::peek_back() const {
	if (tail == nullptr) {
		throw std::runtime_error("List is empty!");
	}
	else {
	return tail->data;
	}
}

//---peek_front()
template <typename E>
E& SSLL<E>::peek_front() {
	E value;
	if (head == nullptr) {
		throw std::runtime_error("List is empty!");
	}
	else {
	return head->data;
	}
}

//---peek_front() const
template <typename E>
const E& SSLL<E>::peek_front() const {
	E value;
	if (head == nullptr) {
		throw std::runtime_error("List is empty!");
	}
	else {
	return head->data;
	}
}

//---is_empty()
template <typename E>
bool SSLL<E>::is_empty() {
	if (head == nullptr) {
		//std::cout<<"Empty!"<<std::endl;
		return true;
	}
	else {
		//std::cout<<"Not Empty!"<<std::endl;
		return false;
	}
}

//---is_full()
template <typename E>
bool SSLL<E>::is_full() {
	//std::cout<<"If this is full you have a problem"<<std::endl;
	return false;
}

//---length()
template <typename E>
size_t SSLL<E>::length() {
	class Node<E> *temp;
	temp = head;
	size_t length = 0;
	while (temp != nullptr) {
		temp = temp->next;
		length++;
	}
	return length;
}

//---length() const
template <typename E>
const size_t SSLL<E>::length() const {
	class Node<E> *temp;
	temp = head;
	size_t length = 0;
	while (temp != nullptr) {
		temp = temp->next;
		length++;
	}
	return length;
}

//---clear()
template <typename E>
void SSLL<E>::clear() {
	if (head == nullptr) {
		//throw std::runtime_error("List is empty!");
		head = nullptr;
		tail = nullptr;
	}
	else {
		class Node<E> *temp,*prev;
		temp = head;
		while (temp != nullptr) {
			prev = temp;
			temp = temp->next;
			delete prev;
		}
		head = nullptr;
		tail = nullptr;
	}
}

//---contains()
template <typename E>
bool SSLL<E>::contains(E element, bool (*equals_function)(const E&,const E&)) {
	if (head == nullptr) {
		throw std::runtime_error("List is empty!");
		return false;
	}
	else {
		class Node<E> *temp;
		temp = head;
		while (temp != nullptr) {
			if (equals_function(element,temp->data)) {
				//std::cout<<element<<" exists in list!"<<std::endl;
				return true;
			}
			temp = temp->next;
		}
		//std::cout<<element<<" is not in list!"<<std::endl;
		return false;
	}
}

//---print()
template <typename E>
void SSLL<E>::print(std::ostream& stream) {
	if (head == nullptr) {
		stream <<"<empty list>"<< std::endl;
	}
	else {
		class Node<E> *temp;
		temp = head;
		stream<<"["<<temp->data;
		temp = temp->next;
		while (temp != nullptr) {
			stream<<","<<temp->data;
			temp = temp->next;
		}
		stream<<"]"<<std::endl;
	}
}

//---contents()
template <typename E>
E* const SSLL<E>::contents() {
	if (head == nullptr) {
		throw std::runtime_error("List is empty!");
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
	
	//print new array for testing purposes
	/*
	for (i = 0; i < size; i++) {
		std::cout<<array[i]<<" ";
	}
	*/
	
	return array;
}


}
#endif
