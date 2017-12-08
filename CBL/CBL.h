//Circular Buffer list
//implements the Linked List using a circular backing array

//currently performance is down because of calling length function in most operations
//this is because Dave doesn't want use saving length as a variable

//maybe change all the ints to size_t later?

//good luck to the poor soul who tries to understand this code later
//(that includes me)

#ifndef _CBL_H_
#define _CBL_H_
#include <stdexcept>
#include "List.h"

namespace cop3530 {

template <typename E>
class CBL : public List<E> {
	public:
	CBL(size_t size = 50);
	~CBL() override;
	CBL(const CBL& other); //copy constructor
	CBL<E>& operator= (const CBL& other); //copy-assignment operator
	CBL(CBL&& other); //move constructor
	CBL<E>& operator= (CBL&& other); //move-assignment operator
	
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
	
	bool contains(E element, bool (*equals_function)(const E&, const E&));	
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
	size_t head = 0;
	size_t tail = 0;
	
	public:
	//iterator stuff
	//iterator points to one past array size, so last 'value' will be garbage data
	template <typename DataT>
	class CBL_Iter {
		public:
		//type aliases required for C++ iterator compatibility
		//using size_t = std::size_t;
		using value_type = DataT;
		using reference = DataT&;
		using pointer = DataT*;
		using difference_type = std::ptrdiff_t;
		using iterator_category = std::forward_iterator_tag;
		
		//type aliases for prettier(!) code
		using self_type = CBL_Iter;
		using self_reference = CBL_Iter&;
		
		private:
		//is the iterator going to have to have its own array?
		DataT * iter_array;
		DataT * end;
		DataT * begin;
		
		
		public:
		//constructor
		explicit CBL_Iter(DataT * array = nullptr, DataT * end = nullptr, DataT * begin = nullptr) : iter_array(array), end(end), begin(begin) {}
		
		//operations
		reference operator*() const {
			return *iter_array;
		}
		pointer operator->() const {
			return &(operator*());
		}
		
		//copy and assignment
		CBL_Iter( const CBL_Iter& src) : iter_array(src.iter_array), end(src.end), begin(src.begin) {}
		self_reference operator = ( CBL_Iter<DataT> const& src) {
			if (this == &src) {
				return (*this);
			}
			iter_array = src.iter_array;
			end = src.end;
			begin = src.begin;
			return *this;
		}
		//preincrement
		//i'm a mother-flippin genius for figuring out this iterator!
		self_reference operator++() {
			++iter_array;
			if (iter_array == end) {
				iter_array = begin;
			}
			return *this;
		} 
		//postincrement
		self_type operator++(int) {
			self_type tmp = *this;
			++(*this);
			return tmp;
		} 
		
		bool operator == ( CBL_Iter<DataT> const& rhs) const {
			return (iter_array == rhs.iter_array);
		}
		bool operator != ( CBL_Iter<DataT> const& rhs) const {
			return (iter_array != rhs.iter_array);
		}
	}; //end of iter
	
	public: 
	//iter type aliases after iter code
	using iterator = CBL_Iter<E>;
	using const_iterator = CBL_Iter<E const>;
	
	//methods to create iters
	//they actualy iterate circularly! neat!
	iterator begin() {
		iterator i(array + head, array + (array_size), array);
		return i;
	}
	iterator end() { 
		iterator i(array+tail, array + (array_size), array);
		return i;
	}
	
	const_iterator begin() const {
		const_iterator i(array + head, array + (array_size), array);
		return i;
	}
	const_iterator end() const {
		const_iterator i(array+tail, array + (array_size), array);
		return i;
	}
};

//---constructors and destructors
template <typename E>
CBL<E>::CBL(size_t size) {
	//set private size variable
	//init array of that size
	//default to 50 if none given
	//need to add 1 slot to size to make it circular 
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
CBL<E>::~CBL() {
	delete[] array;
}

//---copy constructor
template <typename E>
CBL<E>::CBL(const CBL& other) {
	//get array size of other
	//set self array to that size and copy values
	init_size = other.init_size;
	array_size = other.array_size;
	array = new E[array_size];
	for (int i = 0; i < array_size; i++) {
		array[i] = other.array[i];
	}
	tail = other.tail;
	head = other.head;
}

//---copy-assignment
//may have exception problems if other throws exceptions
template <typename E>
CBL<E>& CBL<E>::operator=(const CBL& other) {
	//make sure we aren't referencing ourself
	//clear current array, set default values, then copy
	if (this != &other) {
		delete[] array;
		tail = 0;
		head = 0;
		array_size = 0;
		init_size = 0;
		
		init_size = other.init_size;
		array_size = other.array_size;
		array = new E[array_size];
		for (int i = 0; i < array_size; i++) {
			array[i] = other.array[i];
		}
		tail = other.tail;
		head = other.head;
	}
	return *this;
}

//---move constructor
template <typename E>
CBL<E>::CBL(CBL&& other) {
		array = other.array;
		array_size = other.array_size;
		tail = other.tail;
		head = other.head;
	
		//set others values to default
		delete[] other.array;
		other.array_size = 0;
		other.tail = 0;
		other.head = 0;
}

//---move-assignment
template <typename E>
CBL<E>& CBL<E>::operator=(CBL&& other) {
	//make sure we aren't referencing ourself
	if (this != &other) {
		//free and default ourself;
		delete array;
		array_size = 0;
		tail = 0;
		head = 0;
		
		array = other.array;
		array_size = other.array_size;
		tail = other.tail;
		head = other.head;
	
		//set others values to default
		delete[] other.array;
		other.array_size = 0;
		other.tail = 0;
		other.head = 0;
	}
	return *this;
}

//---push_back()
template <typename E>
void CBL<E>::push_back(E element) {
	//check for errors
	size_t length = this->length();
	if (length == array_size - 1) {
		this->allocate_new();
	}
	
	tail++;
	//if we hit end of array, we need to wrap around
	if (tail > array_size) {
		tail = 1;
	}
	array[tail-1] = element;
	
	//std::cout<<"Pushed to back!"<<std::endl;
}

//---push_front()
template <typename E>
void CBL<E>::push_front(E element) {
	//check for errors
	size_t length = this->length();
	if (length == array_size - 1) {
		this->allocate_new();
	}
	
	//if list is empty, push to head but don't change head index yet
	if (head == 0 && tail == 0){
		array[head] = element;
		tail++;
	}
	//wrap around if pushing front goes past beginning of array
	else if (head == 0) {
		head = array_size - 1;
		array[head] = element;
	}
	//otherwise handle normally
	else {
		head--;
		array[head] = element;
	}
	//std::cout<<"Pushed to front!"<<std::endl;
}

//---insert()
template <typename E>
void CBL<E>::insert(E element, size_t pos) {
	//check if invalid index
	size_t length = this->length();
	
	//if list is empty, handle special case;
	if (length == 0) {
		this->push_back(element);
		return;
	}
	if (pos > length || pos < 0) {
		throw std::invalid_argument("Invalid position");
		return;
	}
	
	if (length == array_size - 1) {
		this->allocate_new();
	}
	
	//like a normal array
	if (head <= tail) {
		int i = 0;
		//save last element if we need to wrap around
		E temp = array[tail-1];
		//shift everything up 1
		for (i = tail; i > pos + head; i--) {
			array[i] = array[i - 1];
		}
		tail++;
		array[i] = element;
		
		//wrap around if needed
		if (tail > array_size) {
			tail = 1;
			array[tail-1] = element;
		}
	}
	//otherwise head > tail and it is circular
	else {
		int i = 0;
		E temp = array[array_size - 1];
		//if pos is on head side of circular array, we'll need wrap around when we shift up
		if (pos < array_size - head) {
			for (i = array_size; i > pos + head; i--) {
				array[i] = array[i - 1];
			}
			array[i] = element;
			
			for (i = tail; i > 0; i--) {
				array[i] = array[i-1];
			}
			array[0] = temp;
			tail++;
		}
		//otherwise we only need to shift up at tail end
		else {
		//shift everything up at tail end
			size_t local_pos = pos - (array_size - head);
			for (i = tail; i > local_pos; i--) {
				array[i] = array[i-1];
			}
			array[i] = element;
			tail++;
		}
	}
	//std::cout<<"Inserted!"<<std::endl;
}

//---replace()
template <typename E>
void CBL<E>::replace(E element, size_t pos) {
	//check if invalid index
	size_t length = this->length();
	if (length == 0) {
		throw std::runtime_error("List is empty!");
		return;
	}
	if (pos > length || pos < 0) {
		throw std::invalid_argument("Invalid position");
		return;
	}
	
	//normal array, no wrap around
	if (head <= tail) {
		array[head + pos] = element;
	}
	//otherwise wrap around
	else {
		if (pos < array_size - head) {
			array[head + pos] = element;
		}
		else {
			size_t local_pos = pos - (array_size - head);
			array[local_pos] = element;
		}	
	}
	//std::cout<<"Replaced!"<<std::endl;
}

//---remove()
//does removing shift everything up or down in a CBL?
template <typename E>
E CBL<E>::remove(size_t pos) {
	//check if invalid index 
	size_t length = this->length();
	if (length == 0) {
		throw std::runtime_error("List is empty!");
		return 0;
	}
	if (pos > length || pos < 0) {
		throw std::invalid_argument("Invalid position");
		return 0;
	}
	
	if (array_size >= (init_size * 2) && length < array_size/2) {
		this->allocate_new();
	}
	
	E value;
	//like a normal array
	if (head <= tail) {
		int i = 0;
		value = array[head + pos];
		
		//shift everything down 1
		for (i = pos + head; i < tail; i++) {
			array[i] = array[i + 1];
		}
		tail--;
	}
	//otherwise head > tail and it is circular
	else {
		int i = 0;
		E temp = array[0];
		//if pos is on head side of circular array, we'll need wrap around when we shift up
		if (pos < array_size - head) {
			value = array[head + pos];
			for (i = head; i < array_size; i++) {
				array[i] = array[i + 1];
			}
			
			for (i = 0; i < tail; i++) {
				array[i] = array[i+1];
			}
			array[array_size - 1] = temp;
			tail--;
			
		}
		//otherwise we only need to shift up at tail end
		else {
			//shift everything up at tail end
			size_t local_pos = pos - (array_size - head);
			value = array[local_pos];
			for (i = local_pos; i < tail; i++) {
				array[i] = array[i+1];
			}
			tail--;
		}
	}
	return value;
}

//---pop_back()
template <typename E>
E CBL<E>::pop_back() {
	size_t length = this->length();
	
	if (length == 0) {
		throw std::runtime_error("List is empty!");
		return 0;
	}
	
	if (array_size >= (init_size * 2) && length < array_size/2) {
		this->allocate_new();
	}
	
	//wrap around if tail goes past beginning of list
	if (tail < 0) {
		tail = array_size;
	}
	
	E value = array[tail-1];
	tail--;
	return value;
}

//---pop_front()
template <typename E>
E CBL<E>::pop_front() {
	size_t length = this->length();
	
	if (length == 0) {
		throw std::runtime_error("List is empty!");
		return 0;
	}
	
	if (array_size >= (init_size * 2) && length < array_size/2) {
		this->allocate_new();
	}
	
	//wrap around if we go past array size
	if (head >= array_size) {
		head = 0;
	}
	
	E value = array[head];
	head++;
	return value;
}

//---item_at()
template <typename E>
E& CBL<E>::item_at(size_t pos) {
	//check if invalid index 
	size_t length = this->length();
	
	if (length == 0) {
		throw std::runtime_error("List is empty!");
	}
	
	if (pos >= length || pos < 0) {
		throw std::invalid_argument("Invalid position");
	}

	//normal array, no wrap around
	if (head <= tail) {
		return array[head + pos];
	}
	//otherwise wrap around
	else {
		if (pos < array_size - head) {
			return array[head + pos];
		}
		else {
			size_t local_pos = pos - (array_size - head);
			return array[local_pos];
		}	
	}
}

//---item_at() const
template <typename E>
const E& CBL<E>::item_at(size_t pos) const{
	//check if invalid index 
	size_t length = this->length();
	
	if (length == 0) {
		throw std::runtime_error("List is empty!");
	}
	
	if (pos >= length || pos < 0) {
		throw std::invalid_argument("Invalid position");
	}
	
	//normal array, no wrap around
	if (head <= tail) {
		return array[head + pos];
	}
	//otherwise wrap around
	else {
		if (pos < array_size - head) {
			return array[head + pos];
		}
		else {
			size_t local_pos = pos - (array_size - head);
			return array[local_pos];
		}	
	}
}

//---peek_back()
template <typename E>
E& CBL<E>::peek_back() {
	if (head == tail) {
		throw std::runtime_error("List is empty!");
	}
	return array[tail-1];
}

//---peek_back() const
template <typename E>
const E& CBL<E>::peek_back() const {
	if (head == tail) {
		throw std::runtime_error("List is empty!");
	}
	return array[tail-1];
}

//---peek_front()
template <typename E>
E& CBL<E>::peek_front() {
	if (head == tail) {
		throw std::runtime_error("List is empty!");
	}

	return array[head];
}

//---peek_front() const
template <typename E>
const E& CBL<E>::peek_front() const {
	if (head == tail) {
		throw std::runtime_error("List is empty!");
	}

	return array[head];
}

//---is_empty()
template <typename E>
bool CBL<E>::is_empty() {
	if (head == tail) {
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
bool CBL<E>::is_full() {
	size_t length = this->length();
	if (length == array_size) {
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
size_t CBL<E>::length() {
	size_t length = 0;
	//if head before tail, like a normal array
	if (head <= tail) {
		length = tail - head;
	}
	//if head after tail, there is wrap around, i.e. it is circular
	else {
		length = tail + (array_size - head);
	}
	return length;
}

//---length() const
template <typename E>
const size_t CBL<E>::length() const {
	size_t length = 0;
	//if head before tail, like a normal array
	if (head <= tail) {
		length = tail - head;
	}
	//if head after tail, there is wrap around, i.e. it is circular
	else {
		length = tail + (array_size - head);
	}
	return length;
}

//---clear()
//just defaults all values in array to 0, doesn't delete array
//sets head and tail indices
template <typename E>
void CBL<E>::clear() {
	for (int i = 0; i < array_size; i++){
			array[i] = 0;
	}
	head = 0;
	tail = 0;
}

//---contains()
template <typename E>
bool CBL<E>::contains(E element, bool (*equals_function)(const E&, const E&)) {
	if (tail == head) {
		throw std::runtime_error("List is empty!");
		return false;
	}
	
	//if head before tail, like a normal array
	if (head <= tail) {
		for (int i = head; i < tail; i++) {
			if (equals_function(element,array[i])) {
				//std::cout<<element<<" exists in list!"<<std::endl;
				return true;
			}
		}
	}
	//if head after tail, there is wrap around, i.e. it is circular
	else {
		
		for (int i = head; i < array_size; i++) {
			if (equals_function(element,array[i])) {
				//std::cout<<element<<" exists in list!"<<std::endl;
				return true;
			}
		}
			
		for (int i = 0; i < tail; i++) {
			if (equals_function(element,array[i])) {
				//std::cout<<element<<" exists in list!"<<std::endl;
				return true;
			}
		}
	}
	
	//std::cout<<element<<" is not in list!"<<std::endl;
	return false;
}


//---print()
template <typename E>
void CBL<E>::print(std::ostream& stream) {
	
	if (tail == head) {
		stream<<"<empty list>"<<std::endl;
		return;
	}
	
	stream<<"["<<array[head];
	//if head before tail, like a normal array
	if (head <= tail) {
		for (int i = head + 1; i < tail; i++) {
			stream<<","<<array[i];
		}
	}
	//if head after tail, there is wrap around, i.e. it is circular
	else {
		
		for (int i = head + 1; i < array_size; i++) {
			stream<<","<<array[i];
		}
			
		for (int i = 0; i < tail; i++) {
			stream<<","<<array[i];
		}
	}
	stream<<"]"<<std::endl;
}

//---contents()
template <typename E>
E* const CBL<E>::contents() {
	size_t length = this->length();
	E * copied_Array = new E[length];
	
	//if head before tail, like a normal array
	if (head <= tail) {
		for (int i = head; i < tail; i++) {
			copied_Array[i] = array[i];
		}
	}
	//if head after tail, there is wrap around, i.e. it is circular
	else {
		size_t j = 0;
		for (int i = head; i < array_size; i++) {
			copied_Array[j] = array[i];
			j++;
		}
			
		for (int i = 0; i < tail; i++) {
			copied_Array[j] = array[i];
			j++;
		}
	}
	
	//print array for testing purposes
	/*
	for (int i = 0; i < length; i++) {
		//std::cout<<copied_Array[i]<<" ";
	}
	*/
	
	return copied_Array;
}

//---allocate_new()
template <typename E>
void CBL<E>::allocate_new() {
	//std::cout<<"allocating new array"<<std::endl;
	size_t length = this->length();
	if (array_size >= (init_size * 2) && length < array_size/2) {
		//allocate new array 75% size of original
		//copy items over
		//deallocate original one
		size_t orig_array_size = array_size;
		array_size = array_size * 0.75;
		E * temp = new E[array_size];
		
		//if head before tail, like a normal array
		if (head <= tail) {
			for (int i = head; i < tail; i++) {
				temp[i] = array[i];
			}
		}
		//if head after tail, there is wrap around, i.e. it is circular
		else {
			
			size_t head_dist = orig_array_size - head;
			for (int i = 1; i <= head_dist; i++) {
				temp[array_size - i] = array[orig_array_size - i];
			}
			
			head = array_size - head_dist;
			
			for (int i = 0; i < tail; i++) {
				temp[i] = array[i];
			}
		}
		
		delete[] array;
		array = temp;
	}
	else {
		//allocate new array 150% size of original
		//copy items over
		//deallocate original one
		size_t orig_array_size = array_size;
		array_size = array_size * 1.5; 
		E * temp = new E[array_size];
		
		//if head before tail, like a normal array
		if (head <= tail) {
			for (int i = head; i < tail; i++) {
				temp[i] = array[i];
			}
		}
		//if head after tail, there is wrap around, i.e. it is circular
		else {
			
			size_t head_dist = orig_array_size - head;
			for (int i = 1; i <= head_dist; i++) {
				temp[array_size - i] = array[orig_array_size - i];
			}
			
			head = array_size - head_dist;
			
			for (int i = 0; i < tail; i++) {
				temp[i] = array[i];
			}
		}
		
		delete[] array;
		array = temp;
	}
	//std::cout<<"new array size: "<<array_size<<std::endl;
}

}
#endif
