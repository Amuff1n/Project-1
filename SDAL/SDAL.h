//Simple Dynamic Array-based List
//implements the Linked List using backing array

//maybe change all the ints to size_t later?

// do more testing dingus

//TODO check if list is empty when popping, removing, peeking

#ifndef _SDAL_H_
#define _SDAL_H_
#include <stdexcept>
#include "List.h"

namespace cop3530 {

template <typename E>
class SDAL : public List<E> {
	public:
	SDAL(size_t size);
	~SDAL() override;
	SDAL(const SDAL& other); //copy constructor
	SDAL<E>& operator= (const SDAL& other); //copy-assignment operator
	SDAL(SDAL&& other); //move constructor
	SDAL<E>& operator= (SDAL&& other); //move-assignment operator
	
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
	
	void allocate_new();
	
	bool contains(E element, bool (*equals_function)(const E&,const E&));	
	void print(std::ostream& stream) override;
	E* const contents() override;
	
	//type aliases
	//using size_t = std::size_t;
	using value_type = E;
	
	private: 
	E * array;
	
	size_t array_size; //does NOT store size of list, rather the size of the ARRAY
	size_t init_size; //original user specified size, used for array allocation
	//in this implementation, tail is an index
	//head pointer will always be array[0]
	size_t tail = 0;
	
	public:
	//iterator stuff
	//iterator points to one past array size, so last 'value' will be garbage data
	template <typename DataT>
	class SDAL_Iter {
		public:
		//type aliases required for C++ iterator compatibility
		//using size_t = std::size_t;
		using value_type = DataT;
		using reference = DataT&;
		using pointer = DataT*;
		using difference_type = std::ptrdiff_t;
		using iterator_category = std::forward_iterator_tag;
		
		//type aliases for prettier(!) code
		using self_type = SDAL_Iter;
		using self_reference = SDAL_Iter&;
		
		private:
		//is the iterator going to have to have its own array?
		DataT * iter_array;
		
		public:
		//constructor
		explicit SDAL_Iter(DataT * array = nullptr) : iter_array(array) {}
		
		//operations
		reference operator*() const {
			return *iter_array;
		}
		pointer operator->() const {
			return &(operator*());
		}
		
		//copy and assignment
		SDAL_Iter( const SDAL_Iter& src) : iter_array(src.iter_array) {}
		self_reference operator = ( SDAL_Iter<DataT> const& src) {
			if (this == &src) {
				return (*this);
			}
			iter_array = src.iter_array;
			return *this;
		}
		//preincrement
		self_reference operator++() {
			++iter_array;
			return *this;
		} 
		//postincrement
		self_type operator++(int) {
			self_type tmp = *this;
			++(*this);
			return tmp;
		} 
		
		bool operator == ( SDAL_Iter<DataT> const& rhs) const {
			return (iter_array == rhs.iter_array);
		}
		bool operator != ( SDAL_Iter<DataT> const& rhs) const {
			return (iter_array != rhs.iter_array);
		}
	}; //end of iter
	
	public: 
	//iter type aliases after iter code
	using iterator = SDAL_Iter<E>;
	using const_iterator = SDAL_Iter<E const>;
	
	//methods to create iters
	//iterator end currently points to 1 past array_size
	//because of this it traverses up to declared size of array
	//TODO get size of list before creating end iter
	iterator begin() {
		iterator i(array);
		return i;
	}
	iterator end() { 
		iterator i(array+array_size+1);
		return i;
	}
	
	const_iterator begin() const {
		const_iterator i(array);
		return i;
	}
	const_iterator end() const {
		const_iterator i(array+array_size+1);
		return i;
	}
};

//---constructors and destructors
template <typename E>
SDAL<E>::SDAL(size_t size) {
	//set private size variable
	//init array of that size
	//default to 50 if none given
	if (size == 0) {
		array = new E[50];
		array_size = 50;
		init_size = 50;
	}
	else {
		array = new E[size];
		array_size = size;
		init_size = size;
	}
}

template <typename E>
SDAL<E>::~SDAL() {
	delete[] array;
}

//---copy constructor
template <typename E>
SDAL<E>::SDAL(const SDAL& other) {
	//get array size of other
	//set self array to that size and copy values
	array_size = other.array_size;
	array = new E[array_size];
	for (int i = 0; i < array_size; i++) {
		array[i] = other.array[i];
	}
	tail = other.tail;
}

//---copy-assignment
//may have exception problems if other throws exceptions
template <typename E>
SDAL<E>& SDAL<E>::operator=(const SDAL& other) {
	//make sure we aren't referencing ourself
	//clear current array, set default values, then copy
	if (this != &other) {
		delete[] array;
		tail = 0;
		array_size = other.array_size;
		array = new E[array_size];
		for (int i = 0; i < array_size; i++) {
			array[i] = other.array[i];
		}
		tail = other.tail;
	}
	return *this;
}

//---move constructor
template <typename E>
SDAL<E>::SDAL(SDAL&& other) {
		array = other.array;
		array_size = other.array_size;
		tail = other.tail;
	
		//set others values to default
		delete[] other.array;
		other.array_size = 0;
		other.tail = 0;
}

//---move-assignment
template <typename E>
SDAL<E>& SDAL<E>::operator=(SDAL&& other) {
	//make sure we aren't referencing ourself
	if (this != &other) {
		//free and default ourself;
		delete array;
		array_size = 0;
		tail = 0;
		
		array = other.array;
		array_size = other.array_size;
		tail = other.tail;
	
		//set others values to default
		delete[] other.array;
		other.array_size = 0;
		other.tail = 0;
	}
	return *this;
}

//---push_back()
template <typename E>
void SDAL<E>::push_back(E element) {
	//check for errors
	if (tail >= array_size) {
		this->allocate_new();
	}
	tail++;
	array[tail-1] = element;
	
	//std::cout<<"Pushed to back!"<<std::endl;
}

//---push_front()
template <typename E>
void SDAL<E>::push_front(E element) {
	//check for errors
	if (tail >= array_size) {
		this->allocate_new();
	}
	size_t size = this->length();
	for (int i = size;  i > 0; i--) {
		array[i] = array[i - 1];
	}
	tail++;
	array[0] = element;
	//std::cout<<"Pushed to front!"<<std::endl;
}

//---insert()
template <typename E>
void SDAL<E>::insert(E element, size_t pos) {
	//check if invalid index
	if (pos > tail-1 || pos < 0) {
		throw std::invalid_argument("Invalid position");
		return;
	}
	
	if (tail >= array_size) {
		this->allocate_new();
	}
	
	size_t size = this->length();
	for (int i = size; i > pos; i--) {
		array[i] = array[i - 1];
	}
	tail++;
	array[pos] = element;
	//std::cout<<"Inserted!"<<std::endl;
}

//---replace()
template <typename E>
void SDAL<E>::replace(E element, size_t pos) {
	//check if invalid index
	if (pos > tail-1 || pos < 0) {
		throw std::invalid_argument("Invalid position");
		return;
	}
	
	array[pos] = element;
	//std::cout<<"Replaced!"<<std::endl;
}

//---remove()
template <typename E>
E SDAL<E>::remove(size_t pos) {
	//check if invalid index 
	if (pos > tail-1 || pos < 0) {
		throw std::invalid_argument("Invalid position");
		return 0;
	}
	
	if (array_size >= (init_size * 2) && tail < array_size/2) {
		this->allocate_new();
	}
	
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
	if (array_size >= (init_size * 2) && tail < array_size/2) {
		this->allocate_new();
	}
	
	E value;
	value = array[tail-1];
	tail--;
	return value;
}

//---pop_front()
template <typename E>
E SDAL<E>::pop_front() {
	if (array_size >= (init_size * 2) && tail < array_size/2) {
		this->allocate_new();
	}
	
	size_t size = this->length();
	E value = array[0];
	for (int i = 0; i < size; i++) {
		array[i] = array[i + 1];
	}
	tail--;
	return value;
}

//---item_at()
template <typename E>
E& SDAL<E>::item_at(size_t pos) {
	//check if invalid index 
	if (pos > tail-1 || pos < 0) {
		throw std::invalid_argument("Invalid position");
	}
	return array[pos];
}

//---item_at() const
template <typename E>
const E& SDAL<E>::item_at(size_t pos) const {
	//check if invalid index 
	if (pos > tail-1 || pos < 0) {
		throw std::invalid_argument("Invalid position");
	}
	return array[pos];
}

//---peek_back()
template <typename E>
E& SDAL<E>::peek_back() {
	return array[tail-1];
}

//---peek_back() const
template <typename E>
const E& SDAL<E>::peek_back() const {
	return array[tail-1];
}

//---peek_front()
template <typename E>
E& SDAL<E>::peek_front() {
	return array[0];
}

//---peek_front() const 
template <typename E>
const E& SDAL<E>::peek_front() const {
	return array[0];
}

//---is_empty()
template <typename E>
bool SDAL<E>::is_empty() {
	if (array[0] == 0 && tail == 0) {
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
bool SDAL<E>::is_full() {
	size_t size = this->length();
	if (size == array_size) {
		//std::cout<<"Array is full!"<<std::endl;
		return true;
	}
	else {
		//std::cout<<"Array is not full!"<<std::endl;
		return false;
	}
}

//---length()
template <typename E>
size_t SDAL<E>::length() {
	size_t length = tail;
	return length;
}

//---length() const
template <typename E>
const size_t SDAL<E>::length() const {
	size_t length = tail;
	return length;
}

//---clear()
//just defaults all values in array to 0, doesn't delete array
//sets head and tail indices
template <typename E>
void SDAL<E>::clear() {
	for (int i = 0; i < array_size; i++){
			array[i] = 0;
	}
	tail = 0;
}

//---contains()
template <typename E>
bool SDAL<E>::contains(E element, bool (*equals_function)(const E&,const E&)) {
	if (tail == 0) {
		throw std::runtime_error("List is empty!");
		return false;
	}
	for (int i = 0; i <= tail; i++) {
		if (equals_function(element,array[i])) {
			//std::cout<<element<<" exists in list!"<<std::endl;
			return true;
		}
	}
	//std::cout<<element<<" is not in list!"<<std::endl;
	return false;
}


//---print()
template <typename E>
void SDAL<E>::print(std::ostream& stream) {
	if (tail == 0) {
		stream<<"<empty list>"<<std::endl;
		return;
	}
	stream<<"[ ";
	for (int i = 0; i < tail; i++) {
		stream<<array[i]<<", ";
	}
	stream<<"]"<<std::endl;
	//std::cout<<"array_size: "<<array_size<<std::endl;
}

//---contents()
template <typename E>
E* const SDAL<E>::contents() {
	E * copied_Array = new E[tail];
	for (int i = 0; i < tail; i++) {
		copied_Array[i] = array[i];
	}
	
	//print array for testing purposes
	/*
	for (i = 0; i < size; i++) {
		//std::cout<<array[i]<<" ";
	}
	*/
	
	return copied_Array;
}

//---allocate_new()
//allocate new array with new size
template <typename E>
void SDAL<E>::allocate_new() {
	if (array_size >= (init_size * 2) && tail < array_size/2) {
		//allocate new array 75% size of original
		//copy items over
		//deallocate original one
		array_size = array_size * 0.75;
		E * temp = new E[array_size];
		for (int i = 0; i < tail; i++) {
			temp[i] = array[i];
		}
		delete[] array;
		array = temp;
	}
	else {
		//allocate new array 150% size of original
		//copy items over
		//deallocate original one
		array_size = array_size * 1.5; 
		E * temp = new E[array_size];
		for (int i = 0; i < tail; i++) {
			temp[i] = array[i];
		}
		delete[] array;
		array = temp;
	}
}

}
#endif
