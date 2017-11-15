#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "CBL.h"

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
	}
}