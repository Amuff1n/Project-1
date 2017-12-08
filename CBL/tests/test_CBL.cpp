#define CATCH_CONFIG_MAIN
#include "../catch.hpp"

#include "../CBL.h"

#include <iostream>

using namespace cop3530;

//equals function declared here
//returns true if both input values are equals
bool equals_function (const int& one, const int& two) {
	if (one == two) {
		return true;
	}
	else {
		return false;
	}
}

SCENARIO("Simple Test of Methods") {
	GIVEN ("List of integers and initial array size of 5") {
		
		List<int> * cbl = new CBL<int>(5);
		
		cbl->push_back(3);
		cbl->push_front(1);
		cbl->insert(0,0);
		cbl->insert(2,2);
		cbl->print(std::cout);
		
		WHEN ("Length checked") {
			size_t length = cbl->length();
			THEN ("Length should be 4") {
				REQUIRE(length == 4);
			}
		}
		
		WHEN("Checking emptiness") {
			bool empty = cbl->is_empty();
			THEN("The list should not be empty") {
				REQUIRE(empty == false);
			}
		}
		
		WHEN("Peeking front") {
			size_t front = cbl->peek_front();
			THEN ("Front should be 0") {
				REQUIRE(front == 0);
			}
		}
		
		WHEN("Peeking back") {
			size_t back = cbl->peek_back();
			THEN ("Back should be 3") {
				REQUIRE(back == 3);
			}
		}
		
		WHEN("Peeking at position 1") {
			size_t item = cbl->item_at(1);
			THEN("Item should 1") {
				REQUIRE(item == 1);
			}
		}
		
		WHEN("Returning contents of list as array") {
			int * temp_array = cbl->contents();
			size_t length = cbl->length();
			THEN ("Array should be [0,1,2,3]") {
				size_t array[4] = {0,1,2,3};
				for(size_t i = 0; i < length; i++) {
					REQUIRE(temp_array[i] == array[i]);
				}
			}
			//delete that dynamic array, we dont like mem leaks
			delete[] temp_array;
		}
		
		WHEN("Replacing position 2 with the number 4") {
			cbl->replace(4,2);
			size_t item = cbl->item_at(2);
			THEN ("Item at position 2 should be 4") {
				REQUIRE(item == 4);
			}
		}
		
		WHEN("Removing position 0") {
			size_t item = cbl->remove(0);
			THEN("The value should be 0") {
				REQUIRE(item == 0);
			}
		}
		
		WHEN("Popping front") {
			size_t front = cbl->pop_front();
			THEN("The front should be 1") {
				REQUIRE(front == 0);
			}
		}
		
		WHEN("Popping back") {
			size_t back = cbl->pop_back();
			THEN("The back should be 3") {
				REQUIRE(back == 3);
			}
		}
		
		WHEN("Clearing the list") {
			cbl->clear();
			size_t length = cbl->length();
			THEN("The length should be 0") {
				REQUIRE(length == 0);
			}
		}
		
		WHEN("Checking if the list contains the value 3") {
			bool contains = cbl->contains(3,equals_function);
			THEN("Should return true") {
				REQUIRE(contains == true);
			}
		}
		
		//clean up
		delete cbl;
		
	}
}

SCENARIO("Testing circular functionality") {
	GIVEN("List of integers with head and tail index at end of array of init size 5") {
		List<int> * cbl = new CBL<int>(5);
		
		cbl->push_back(0);
		cbl->push_back(1);
		cbl->pop_front();
		cbl->push_back(2);
		cbl->pop_front();
		cbl->push_back(3);
		cbl->pop_front();
		cbl->push_back(4);
		cbl->pop_front();
		cbl->print(std::cout);
		
		WHEN("Tail index is at end of array and we push 5 back") {
			cbl->push_back(5);
			size_t tail = cbl->peek_back();
			THEN("Tail value should be 5") {
				REQUIRE(tail == 5);
			}
		}
		
		WHEN("Head (and tail) index is at end of array and we pop front (after pushing 5) ") {
			cbl->push_back(5);
			cbl->pop_front();
			size_t front = cbl->pop_front();
			THEN ("The value returned should be 5") {
				REQUIRE(front == 5);
			}
		}
		
		WHEN("Inserting the value 5 at position 1") {
			cbl->insert(5,1);
			size_t item = cbl->item_at(1);
			THEN("The value at pos 1 should be 5") {
				REQUIRE(item == 5);
			}
		}
		
		delete cbl;
	}
}

SCENARIO("Testing new array allocation (sizing up)") {
	GIVEN("List of integers and initial array size of 5") {
		
		List<int> * cbl = new CBL<int>(5);
		
		cbl->push_back(3);
		cbl->push_front(1);
		cbl->insert(0,0);
		cbl->insert(2,2);
		cbl->print(std::cout);
		
		WHEN("Pushing 4 to back") {
			cbl->push_back(4);
			size_t item = cbl->peek_back();
			THEN ("The value should be 4") {
				REQUIRE(item == 4);
			}
		}
		
		WHEN("Pushing 4 and 5 to back") {
			cbl->push_back(4);
			cbl->push_back(5);
			size_t item = cbl->peek_back();
			THEN("The value should be 5") {
				REQUIRE(item == 5);
			}
		}
		
		WHEN("Inserting the value 7 into position 1") {
			cbl->insert(7,1);
			size_t item = cbl->peek_back();
			THEN("The back should be 3") {
				REQUIRE(item == 3);
			}
		}
		
		delete cbl;
	}
}

SCENARIO("Testing new array allocation (sizing down)") {
	GIVEN("List of integers, initial array size of 3") {
		
		List<int> * cbl = new CBL<int>(3);
		
		cbl->push_back(2);
		cbl->push_front(1);
		cbl->insert(0,0);
		cbl->print(std::cout);
		
		WHEN("We raise the size of array above 6 and pop back 4 slots") {
			cbl->push_back(3);
			cbl->push_back(4);
			cbl->push_back(5);
			cbl->push_back(6);
			cbl->pop_back();
			cbl->pop_back();
			cbl->pop_back();
			cbl->pop_back();
			size_t item = cbl->pop_back();
			THEN("The back of the list should be 2") {
				REQUIRE(item == 2);
			}
		}
		delete cbl;
	}
}

SCENARIO("Testing 'big five' and iterators") {
	GIVEN ("List of integers") {
		
		//declaring CBL here, not List
		//TODO put virtual iterator methods (and maybe virtual 'big five') in List.h
		CBL<int> cbl(5);
		
		cbl.push_back(3);
		cbl.push_front(1);
		cbl.insert(0,0);
		cbl.insert(2,2);
		cbl.print(std::cout);
		
		WHEN("Testing iterators") {
			CBL<int>::iterator iter = cbl.begin();
			CBL<int>::iterator end = cbl.end();
			THEN("Iterator should return list") {
				size_t array[4] = {0,1,2,3};
				for (int i = 0; iter != end; ++iter, ++i) {
					REQUIRE(*iter == array[i]);
				}
			}
		}
		
		WHEN("Testing copy constructor") {
			CBL<int> cbl2(cbl);
			THEN("List should be same for new copy") {
				for (int i = 0; i < cbl2.length(); i++) {
					REQUIRE(cbl2.item_at(i) == cbl.item_at(i));
				}
			}
		}

		WHEN("Testing copy assignment") {
			CBL<int> cbl2;
			cbl2.push_back(24);
			cbl2.push_front(23);
			
			cbl2 = cbl;
			
			THEN("List should be same for new copy") {
				for (int i = 0; i < cbl2.length(); i++) {
					REQUIRE(cbl2.item_at(i) == cbl.item_at(i));
				}
			}
		}
		
		WHEN("Testing move constructor") {
			CBL<int> cbl2(std::move(cbl));
			THEN("List should move to new copy") {
				size_t array[4] = {0,1,2,3};
				for (int i = 0; i < cbl2.length(); i++) {
					REQUIRE(array[i] == cbl2.item_at(i));
				}
			}
		}
	
		WHEN("Testing move assignment") {
			CBL<int> cbl2;
			cbl2.push_back(24);
			cbl2.push_front(23);
			
			cbl2 = std::move(cbl);
			THEN("List should move to new copy") {
				size_t array[4] = {0,1,2,3};
				for (int i = 0; i < cbl2.length(); i++) {
					REQUIRE(array[i] == cbl2.item_at(i));
				}
			}
		}
	}
}

SCENARIO("Testing large lists") {
	GIVEN("A list of 150 ints") {
		List<int> * cbl = new CBL<int>(5);
		for (int i = 0; i < 150; i++) {
			cbl->push_back(i);
		}
		
		WHEN("Checking length") {
			size_t length = cbl->length();
			THEN("Length should be 150") {
				REQUIRE(length == 150);
			}
		}
		
		WHEN("Popping front") {
			int value = cbl->pop_front();
			size_t length = cbl->length();
			THEN("The value should be 0 and the length should be 149") {
				REQUIRE(value == 0);
				REQUIRE(length == 149);
			}	
		}
		
		WHEN("Popping back") {
			int value = cbl->pop_back();
			size_t length = cbl->length();
			THEN("The value should be 149 and the length should be 149") {
				REQUIRE(value == 149);
				REQUIRE(length == 149);
			}	
		}
		
		WHEN("Removing int from pos 50") {
			int value = cbl->remove(50);
			size_t length = cbl->length();
			THEN("The value should be 50 and length should be 149") {
				REQUIRE(value == 50);
				REQUIRE(length == 149);
			}
		}
		
		WHEN("Inserting 255 at pos 50") {
			cbl->insert(255, 50);
			int value = cbl->item_at(50);
			size_t length = cbl->length();
			THEN("The value at pos 50 should be 255 and the length should be 151") {
				REQUIRE(value == 255);
				REQUIRE(length == 151);
			}
		}
		
		WHEN("Clearing the list") {
			cbl->clear();
			bool x = cbl->is_empty();
			size_t length = cbl->length();
			THEN("Length should be 0 and is_empty() should be true") {
				REQUIRE(x == true);
				REQUIRE(length == 0);
			}
		}
		
		WHEN("Checking contains for 177") {
			bool x = cbl->contains(177,equals_function);
		}
		
		delete cbl;
	} 
}

SCENARIO ("Testing constant-ness(?)") {
	GIVEN ("A constant list") {
		CBL<int> * cbl = new CBL<int>(5);
		
		cbl->push_back(3);
		cbl->push_front(1);
		cbl->insert(0,0);
		cbl->insert(2,2);
		cbl->print(std::cout);
		
		const CBL<int> * cbl2 = cbl; 
		
		WHEN("Checking its length") {
			size_t size = cbl2->length();
			THEN ("Length should be returned as 4") {
				REQUIRE(size == 4);
			}
		}
		
		WHEN("Checking the first item") {
			int value = cbl2->peek_front();
			THEN("The value should be 0") {
				REQUIRE(value == 0);
			}
		}
		
		//TODO iterators will be off from array because they go over entire array instead of list currently
		WHEN("Testing iterators") {
			CBL<int>::const_iterator iter = cbl2->begin();
			CBL<int>::const_iterator end = cbl2->end();
			THEN("Iterator should return list") {
				size_t array[4] = {0,1,2,3};
				for (int i = 0; iter != end; ++iter, ++i) {
					REQUIRE(*iter == array[i]);
				}
			}
		}

		delete cbl;
	}
}
