//Chained Dynamic Array-based list
//combines the CDAL and the CDAL
//chain of nodes where each node has an array of data

//nodes have their own head and tail indices they keep track of
//pushing to front does not shift all elements to the right like in a normal array
//this is because a node's head index may not always be 0 and pushing front goes to the head index of a node
//if we hit the start of the array and want to push to front, we create a new node as the head node and push to that

//this will make pushing and popping front an O(1) operation instead of O(n) like the CDAL

//maybe change all the ints to size_t later?

#ifndef _CDAL_H_
#define _CDAL_H_
#include <stdexcept>
#include "List.h"

namespace cop3530 {
	
template <typename E>
class Node {
	public:
	E  * array; //for CDAL, nodes will have a pointer to an array instead of just a value
	Node<E> * next;
	//Nodes will have their own tail indices but will not have their own operations
	size_t tail_index = 0;
};

template <typename E>
class CDAL : public List<E> {
	public:
	CDAL();
	~CDAL() override;
	CDAL(const CDAL& other); //copy constructor
	CDAL<E>& operator= (const CDAL& other); //copy-assignment operator
	CDAL(CDAL&& other); //move constructor
	CDAL<E>& operator= (CDAL&& other); //move-assignment operator
	
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
	
	bool contains(E element, bool (*equals_function)(const E&, const E&));	
	void print(std::ostream& stream) override;
	E* const contents() override;
	
	
	private: 
	//head and tail pointers for the linked list
	Node<E> * head = nullptr;
	Node<E> * tail = nullptr;
	
	public:
	//Iterator currently works for at least one node
	
	template <typename DataT>
	class CDAL_Iter {
		public:
		//type aliases required for C++ iterator compatibility
		//using size_t = std::size_t;
		using value_type = DataT;
		using reference = DataT&;
		using pointer = DataT*;
		using difference_type = std::ptrdiff_t;
		using iterator_category = std::forward_iterator_tag;
		
		//type aliases for prettier(!) code
		using self_type = CDAL_Iter;
		using self_reference = CDAL_Iter&;
		
		private:
		Node<E> * here;
		DataT * iter_array;
		
		public:
		//constructor
		explicit CDAL_Iter(Node<E> * start = nullptr, DataT * array = nullptr) : here(start), iter_array(array) {}
		
		//operations
		reference operator*() const {
			return *iter_array;
		}
		pointer operator->() const {
			return &(operator*());
		}
		
		//copy and assignment
		CDAL_Iter( const CDAL_Iter& src) : here (src.here), iter_array(src.iter_array) {}
		self_reference operator = ( CDAL_Iter<DataT> const& src) {
			if (this == &src) {
				return (*this);
			}
			here = src.here;
			iter_array = src.iter_array;
			return *this;
		}
		//preincrement
		self_reference operator++() {
			if (here) {
				if (here->tail_index == 50) {
					here = here->next;
					iter_array = here->array;
				}
				++iter_array;
			}
			return *this;
		} 
		//postincrement
		self_type operator++(int) {
			self_type tmp = *this;
			++(*this);
			return tmp;
		} 
		
		bool operator == ( CDAL_Iter<DataT> const& rhs) const {
			return (here == rhs.here && iter_array = rhs.iter_array);
		}
		bool operator != ( CDAL_Iter<DataT> const& rhs) const {
			return (here != rhs.here || iter_array != rhs.iter_array);
		}
	};
	
	public: 
	//iter type aliases after iter code
	using iterator = CDAL_Iter<E>;
	using const_iterator = CDAL_Iter<E const>;
	
	//methods to create iters
	iterator begin() {
		iterator i(head, head->array);
		return i;
	}
	iterator end() { 
		iterator i(tail, tail->array+50);
		return i;
	}
	
	const_iterator begin() const {
		const_iterator i(head, head->array);
		return i;
	}
	const_iterator end() const {
		const_iterator i(tail, tail->array+50);
		return i;
	}
};

//---constructors and destructors
template <typename E>
CDAL<E>::CDAL() {
	//nothing to really init
	//set head and tail pointers to 0
	head = nullptr;
	tail = nullptr;
}

template <typename E>
CDAL<E>::~CDAL() {
	this->clear();
}

//---copy constructor
template <typename E>
CDAL<E>::CDAL(const CDAL& other) {
	class Node<E> *temp;
	temp = other.head;
	while (temp) {
		for (int i = 0; i < temp->tail_index; i++) {
			this->push_back(temp->array[i]);
		}
		temp = temp->next;
	}
}

//---copy-assignment
//may have exception problems if other throws exceptions
template <typename E>
CDAL<E>& CDAL<E>::operator=(const CDAL& other) {
	//make sure we aren't referencing ourself
	if (this != &other) {
		this->clear();
		class Node<E> *temp;
		temp = other.head;
		while (temp) {
			for (int i = 0; i < temp->tail_index; i++) {
				this->push_back(temp->array[i]);
			}
			temp = temp->next;
		}
	}
	return *this;
}

//---move constructor
template <typename E>
CDAL<E>::CDAL(CDAL&& other) {
	head = other->head;
	tail = other->tail;
	//set other to default values to avoid being automatically destroyed
	other->head = nullptr;
	other->tail = nullptr;
}

//---move-assignment
template <typename E>
CDAL<E>& CDAL<E>::operator=(CDAL&& other) {
	//make sure we aren't referencing ourself
	if (this != &other) {
		//free existing data
		this->clear();
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
Node<E> * CDAL<E>::new_node(E element) {
	class Node<E> *temp_Node;
	temp_Node = new Node<E>;
	temp_Node->array = new E[50];
	if (head == nullptr && tail == nullptr) {
		head = temp_Node;
		tail = temp_Node;
	}
	temp_Node->tail_index = 0;
	temp_Node->next = nullptr;
	
	return temp_Node;
}

//---push_back()
template <typename E>
void CDAL<E>::push_back(E element) {
	//check for errors
	class Node<E> *temp_Node;
	temp_Node = tail;
	//if our list has no nodes, make an initial one
	if (temp_Node == nullptr) {
		temp_Node = this->new_node(element);
	}
	//if current node array is full, make new Node at tail
	if (temp_Node->tail_index == 50) {
		temp_Node = this->new_node(element);
		tail->next = temp_Node;
		tail = temp_Node;
	}
	
	tail->tail_index++;
	
	tail->array[tail->tail_index-1] = element;
	//std::cout<<"Pushed to back!"<<std::endl;
}

//---push_front()
template <typename E>
void CDAL<E>::push_front(E element) {
	class Node<E> *temp_Node;
	temp_Node = head;
	//if list empty need new node
	if (temp_Node == nullptr) {
		temp_Node = this->new_node(element);
	} 
	//if current node array is full, make new Node at tail
	//save last value and make it first value of new node
	if (temp_Node->tail_index == 50) {
		if (tail->tail_index == 50) {
		temp_Node = this->new_node(element);
		tail->next = temp_Node;
		tail = temp_Node;
		}
		
		//while a node exists
		//save last value of current node
		//shift everything up 
		//push element to front
		//new element to push to front for next node = saved last value
		temp_Node = head;
		while (temp_Node) {
			
			E temp = temp_Node->array[49];
		
			for (int i = 49;  i > 0; i--) {
				temp_Node->array[i] = temp_Node->array[i - 1];
			}
			temp_Node->array[0] = element;
			element = temp;
			temp_Node = temp_Node->next;
		}
		temp_Node->tail_index++;
	}
	else {
		//otherwise there is space in head node
		//just push front like a normal array
		for (int i = 49;  i > 0; i--) {
			temp_Node->array[i] = temp_Node->array[i - 1];
		}
		temp_Node->array[0] = element;
		temp_Node->tail_index++;
	}
	//std::cout<<"Pushed to front!"<<std::endl;
}

//---insert()
template <typename E>
void CDAL<E>::insert(E element, size_t pos) {
	//check if pos will be invalid
	size_t length = this->length();
	if (pos > length-1 || pos < 0) {
		throw std::invalid_argument("Invalid position");
		return;
	}
	//calculate how many nodes we have to traverse to get to pos
	int node_number = pos/50;
	//mod for local pos 
	int local_pos = pos % 50;
	class Node<E> *temp_Node;
	temp_Node = head;
	//navigate to correct node for indicated pos
	for (int i = 0; i < node_number; i++) {
		temp_Node = temp_Node->next;
	}
	//if no node there, allocate new
	if (temp_Node == nullptr) {
		temp_Node = this->new_node(element);
	}
	
	if (temp_Node->tail_index == 50) {
		//if all nodes are full when we insert, we'll need a new node
		if (tail->tail_index == 50) {
			tail->next = this->new_node(element);
			tail = tail->next;
		}
		
		//insert at correct node
		E temp = temp_Node->array[49];
		
		for (int i = 49;  i > local_pos; i--) {
			temp_Node->array[i] = temp_Node->array[i - 1];
		}
		temp_Node->array[local_pos] = element;
		element = temp;
		temp_Node = temp_Node->next;
			
		//push up and fix rest of nodes
		while (temp_Node) {
			temp = temp_Node->array[49];
		
			for (int i = 49;  i > 0; i--) {
				temp_Node->array[i] = temp_Node->array[i - 1];
			}
			temp_Node->array[0] = element;
			element = temp;
			if (temp_Node->next == nullptr) {
				temp_Node->tail_index++;
			}
			temp_Node = temp_Node->next;
		}
	}
	else {
		//process similar to pushing front but at certain pos 
		for (int i = 49; i > local_pos; i--) {
			temp_Node->array[i] = temp_Node->array[i - 1];
		}
		temp_Node->array[local_pos] = element;
		temp_Node->tail_index++;
	}
	
	//std::cout<<"inserted!"<<std::endl;
}


//---replace()
template <typename E>
void CDAL<E>::replace(E element, size_t pos) {
	class Node<E> *temp_Node;
	temp_Node = head;
	if (head == nullptr) {
		throw std::runtime_error("List is empty!");
		return;
	}
	//check if pos will be invalid
	size_t length = this->length();
	if (pos > length-1 || pos < 0) {
		throw std::invalid_argument("Invalid position");
		return;
	}
	//calculate how many nodes we have to traverse to get to pos
	int node_number = pos/50;
	//mod for local pos 
	int local_pos = pos % 50;
	//navigate to correct node for indicated pos
	for (int i = 0; i < node_number; i++) {
		temp_Node = temp_Node->next;
	}
	
	temp_Node->array[local_pos] = element;
	
	//std::cout<<"Replaced!"<<std::endl;
}

//---remove()
template <typename E>
E CDAL<E>::remove(size_t pos) {
	class Node<E> *temp_Node;
	temp_Node = head;
	if (head == nullptr) {
		throw std::runtime_error("List is empty!");
		return 0;
	}
	//check if pos will be invalid
	size_t length = this->length();
	if (pos > length-1 || pos < 0) {
		throw std::invalid_argument("Invalid position");
		return 0;
	}
	//calculate how many nodes we have to traverse to get to pos
	int node_number = pos/50;
	//mod for local pos 
	int local_pos = pos % 50;
	//navigate to correct node for indicated pos
	for (int i = 0; i < node_number; i++) {
		temp_Node = temp_Node->next;
	}
	
	//if current node is full and next node exists, we'll have to shift values
	if (temp_Node->tail_index == 50 && (temp_Node->next)) {
		E value = temp_Node->array[local_pos];
		for (int i = local_pos; i < 50; i++) {
			temp_Node->array[i] = temp_Node->array[i + 1];
		}
		while (temp_Node->next) {
			temp_Node->array[49] = temp_Node->next->array[0];
			temp_Node = temp_Node->next;
			for (int i = 0; i < 50; i++) {
				temp_Node->array[i] = temp_Node->array[i + 1];
			}
		}
		temp_Node->tail_index--;
		return value;
	}
	else {
		E value = temp_Node->array[local_pos];
		for (int i = local_pos; i < 50; i++) {
			temp_Node->array[i] = temp_Node->array[i + 1];
		}
		temp_Node->tail_index--;
		return value;
	}
}

//---pop_back()
template <typename E>
E CDAL<E>::pop_back() {
	E value;
	if (head == nullptr || head->tail_index == 0) {
		throw std::runtime_error("List is empty!");
		return 0;
	}
	value = tail->array[tail->tail_index-1];
	tail->tail_index--;
	//have to find new tail if current tail is empty
	if (head != tail && tail->tail_index == 0) {
		class Node<E> *temp_Node;
		temp_Node = head;
		while (temp_Node->next->next != nullptr) {
			temp_Node = temp_Node->next;
		}
		tail = temp_Node;
		delete tail->next;
		tail->next = nullptr;
	}
	
	return value;
}

//---pop_front()
template <typename E>
E CDAL<E>::pop_front() {
	E value;
	class Node<E> *temp_Node;
	temp_Node = head;
	
	if (head == nullptr || head->tail_index == 0) {
		throw std::runtime_error("List is empty!");
		return 0;
	}
	value = head->array[0];
	
	//if head node is full and next node exists, we'll have to shift values
	if (temp_Node->tail_index == 50 && (temp_Node->next)) {
		while (temp_Node->next) {
			temp_Node->array[49] = temp_Node->next->array[0];
			temp_Node = temp_Node->next;
			for (int i = 0; i < 50; i++) {
				temp_Node->array[i] = temp_Node->array[i + 1];
			}
		}
		temp_Node->tail_index--;
		return value;
	}
	else {
		//otherwise, head node is only node
		for (int i = 0; i < 50; i++) {
			temp_Node->array[i] = temp_Node->array[i + 1];
		}
		temp_Node->tail_index--;
		return value;
	}
	
	return value;
}

//---item_at()
template <typename E>
E& CDAL<E>::item_at(size_t pos) {
	class Node<E> *temp_Node;
	temp_Node = head;
	if (head == nullptr) {
		throw std::runtime_error("List is empty!");
	}
	//check if pos will be invalid
	size_t length = this->length();
	if (pos > length-1 || pos < 0) {
		throw std::invalid_argument("Invalid position");
	}
	//calculate how many nodes we have to traverse to get to pos
	int node_number = pos/50;
	//mod for local pos 
	int local_pos = pos % 50;
	//navigate to correct node for indicated pos
	for (int i = 0; i < node_number; i++) {
		temp_Node = temp_Node->next;
	}
	
	return temp_Node->array[local_pos];
}

//---item_at() const
template <typename E>
const E& CDAL<E>::item_at(size_t pos) const {
	class Node<E> *temp_Node;
	temp_Node = head;
	if (head == nullptr) {
		throw std::runtime_error("List is empty!");
	}
	//check if pos will be invalid
	size_t length = this->length();
	if (pos > length-1 || pos < 0) {
		throw std::invalid_argument("Invalid position");
	}
	//calculate how many nodes we have to traverse to get to pos
	int node_number = pos/50;
	//mod for local pos 
	int local_pos = pos % 50;
	//navigate to correct node for indicated pos
	for (int i = 0; i < node_number; i++) {
		temp_Node = temp_Node->next;
	}
	
	return temp_Node->array[local_pos];
}

//---peek_back()
template <typename E>
E& CDAL<E>::peek_back() {
	if (tail == nullptr || head->tail_index == 0) {
		throw std::runtime_error("List is empty!");
	}
	else {
		return tail->array[tail->tail_index-1];
	}
}

//---peek_back() const
template <typename E>
const E& CDAL<E>::peek_back() const {
	if (tail == nullptr || head->tail_index == 0) {
		throw std::runtime_error("List is empty!");
	}
	else {
		return tail->array[tail->tail_index-1];
	}
}

//---peek_front()
template <typename E>
E& CDAL<E>::peek_front() {
	if (head == nullptr || head->tail_index == 0) {
		throw std::runtime_error("List is empty!");
	}
	else {
		return head->array[0];
	}
}

//---peek_front() const
template <typename E>
const E& CDAL<E>::peek_front() const {
	if (head == nullptr || head->tail_index == 0) {
		throw std::runtime_error("List is empty!");
	}
	else {
		return head->array[0];
	}
}

//---is_empty()
template <typename E>
bool CDAL<E>::is_empty() {
	if (head == nullptr || head->tail_index == 0) {
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
bool CDAL<E>::is_full() {
	//std::cout<<"If this is full you have a problem"<<std::endl;
	return false;
}

//---length()
template <typename E>
size_t CDAL<E>::length() {
	class Node<E> *temp_Node;
	temp_Node = head;
	size_t length = 0;
	while (temp_Node != nullptr) {
		length = length + (temp_Node->tail_index);
		temp_Node = temp_Node->next;
	}
	return length;
}

//---length() const
template <typename E>
const size_t CDAL<E>::length() const {
	class Node<E> *temp_Node;
	temp_Node = head;
	size_t length = 0;
	while (temp_Node != nullptr) {
		length = length + (temp_Node->tail_index);
		temp_Node = temp_Node->next;
	}
	return length;
}

//---clear()
template <typename E>
void CDAL<E>::clear() {
	if (head == nullptr) {
		//std::cout<<"List is empty!"<<std::endl;
	}
	else {
		class Node<E> *temp_Node,*prev_Node;
		temp_Node = head;
		while (temp_Node != nullptr) {
			prev_Node = temp_Node;
			temp_Node = temp_Node->next;
			delete[] prev_Node->array;
			delete prev_Node;
		}
		
		head = tail = nullptr;
	}
}

//---contains()
template <typename E>
bool CDAL<E>::contains(E element, bool (*equals_function)(const E&, const E&)) {
	class Node<E> *temp_Node;
	temp_Node = head;
	if (head == nullptr || temp_Node->tail_index == 0) {
		throw std::runtime_error("List is empty!");
		return false;
	}
	
	while (temp_Node != nullptr) {
		for (int i = 0; i < temp_Node->tail_index; i++) {
			if (equals_function(temp_Node->array[i],element)) {
				//std::cout<<element<<" exists in list!"<<std::endl;
				return true;
			}
		}
		temp_Node=temp_Node->next;
	}
	//std::cout<<element<<" is not in list!"<<std::endl;
	return false;
}


//---print()
template <typename E>
void CDAL<E>::print(std::ostream& stream) {
	class Node<E> *temp_Node;
	temp_Node = head;
	if (head == nullptr || temp_Node->tail_index == 0) {
		stream <<"<empty list>"<< std::endl;
	}
	
	stream<<"[ ";
	while (temp_Node != nullptr) {
		for (int i = 0; i < temp_Node->tail_index; i++) {
			stream<<temp_Node->array[i]<<", ";
		}
		temp_Node = temp_Node->next;
	}
	stream<<"]"<<std::endl;
}

//---contents()
template <typename E>
E* const CDAL<E>::contents() {
	class Node<E> *temp_Node;
	temp_Node = head;
	if (head == nullptr || temp_Node->tail_index == 0) {
		throw std::runtime_error("List is empty!");
	}
	
	int size = this->length();
	E *copy_array = new E[size];
	int j = 0;
	
	while (temp_Node != nullptr) {
		for (int i = 0; i < temp_Node->tail_index; i++) {
			copy_array[j] = temp_Node->array[i];
			j++;
		}
		temp_Node=temp_Node->next;
	}
	
	//print array for testing purposes
	/*
	for (int i = 0; i < size; i++) {
		std::cout<<copy_array[i]<<" ";
	}
	*/
	
	return copy_array;
}

}
#endif
