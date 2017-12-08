#define CATCH_CONFIG_MAIN
#include "../catch.hpp"

#include "../PSLL.h"

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
	GIVEN ("List of integers") {
		
		PSLL<int> * psll = new PSLL<int>;
		
		psll->push_back(3);
		psll->push_front(1);
		psll->insert(0,0);
		psll->insert(2,2);
		psll->print(std::cout);
		
		WHEN ("Length checked") {
			size_t length = psll->length();
			THEN ("Length should be 4") {
				REQUIRE(length == 4);
			}
		}
		
		WHEN("Checking emptiness") {
			bool empty = psll->is_empty();
			THEN("The list should not be empty") {
				REQUIRE(empty == false);
			}
		}
		
		WHEN("Peeking front") {
			size_t front = psll->peek_front();
			THEN ("Front should be 0") {
				REQUIRE(front == 0);
			}
		}
		
		WHEN("Peeking back") {
			size_t back = psll->peek_back();
			THEN ("Back should be 3") {
				REQUIRE(back == 3);
			}
		}
		
		WHEN("Peeking at position 1") {
			size_t item = psll->item_at(1);
			THEN("Item should 1") {
				REQUIRE(item == 1);
			}
		}
		
		WHEN("Returning contents of list as array") {
			int * temp_array = psll->contents();
			size_t length = psll->length();
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
			psll->replace(4,2);
			size_t item = psll->item_at(2);
			THEN ("Item at position 2 should be 4") {
				REQUIRE(item == 4);
			}
		}
		
		WHEN("Removing position 0") {
			size_t item = psll->remove(0);
			size_t pool_length = psll->pool_length();
			THEN("The value should be 0 and the free pool length should be 1") {
				REQUIRE(item == 0);
				REQUIRE(pool_length == 1);
			}
		}
		
		WHEN("Popping front") {
			size_t front = psll->pop_front();
			THEN("The front should be 1") {
				REQUIRE(front == 0);
			}
		}
		
		WHEN("Popping back") {
			size_t back = psll->pop_back();
			THEN("The back should be 3") {
				REQUIRE(back == 3);
			}
		}
		
		WHEN("Clearing the list") {
			psll->clear();
			size_t length = psll->length();
			THEN("The length should be 0") {
				REQUIRE(length == 0);
			}
		}
		
		WHEN("Checking if the list contains the value 3") {
			bool contains = psll->contains(3,equals_function);
			THEN("Should return true") {
				REQUIRE(contains == true);
			}
		}
		
		//clean up
		delete psll;
		
	}
}

SCENARIO("Testing 'big five' and iterators") {
	GIVEN ("List of integers") {
		
		//declaring PSLL here, not List
		//TODO put virtual iterator methods (and maybe virtual 'big five') in List.h
		PSLL<int> psll;
		
		psll.push_back(3);
		psll.push_front(1);
		psll.insert(0,0);
		psll.insert(2,2);
		psll.print(std::cout);
		
		WHEN("Testing iterators") {
			PSLL<int>::iterator iter = psll.begin();
			PSLL<int>::iterator end = psll.end();
			THEN("Iterator should return list") {
				size_t array[4] = {0,1,2,3};
				for (int i = 0; iter!= end; ++iter, ++i) {
					REQUIRE(*iter == array[i]);
				}
			}
		}
		
		WHEN("Testing copy constructor") {
			PSLL<int> psll2(psll);
			THEN("List should be same for new copy") {
				for (int i = 0; i < psll2.length(); i++) {
					REQUIRE(psll2.item_at(i) == psll.item_at(i));
				}
			}
		}
		
		WHEN("Testing copy assignment") {
			PSLL<int> psll2;
			psll2.push_back(24);
			psll2.push_front(23);
			
			psll2 = psll;
			
			THEN("List should be same for new copy") {
				for (int i = 0; i < psll2.length(); i++) {
					REQUIRE(psll2.item_at(i) == psll.item_at(i));
				}
			}
		}
		
		WHEN("Testing move constructor") {
			PSLL<int> psll2(std::move(psll));
			THEN("List should move to new copy") {
				size_t array[4] = {0,1,2,3};
				for (int i = 0; i < psll2.length(); i++) {
					REQUIRE(array[i] == psll2.item_at(i));
				}
			}
		}
	
		WHEN("Testing move assignment") {
			PSLL<int> psll2;
			psll2.push_back(24);
			psll2.push_front(23);
			
			psll2 = std::move(psll);
			THEN("List should move to new copy") {
				size_t array[4] = {0,1,2,3};
				for (int i = 0; i < psll2.length(); i++) {
					REQUIRE(array[i] == psll2.item_at(i));
				}
			}
		}
	}
}

SCENARIO("Testing large lists") {
	GIVEN("A list of 150 ints") {
		PSLL<int> * psll = new PSLL<int>;
		for (int i = 0; i < 150; i++) {
			psll->push_back(i);
		}
		
		WHEN("Checking length") {
			size_t length = psll->length();
			THEN("Length should be 150") {
				REQUIRE(length == 150);
			}
		}
		
		WHEN("Popping front") {
			int value = psll->pop_front();
			size_t length = psll->length();
			THEN("The value should be 0 and the length should be 149") {
				REQUIRE(value == 0);
				REQUIRE(length == 149);
			}	
		}
		
		WHEN("Popping back") {
			int value = psll->pop_back();
			size_t length = psll->length();
			THEN("The value should be 149 and the length should be 149") {
				REQUIRE(value == 149);
				REQUIRE(length == 149);
			}	
		}
		
		WHEN("Removing int from pos 50") {
			int value = psll->remove(50);
			size_t length = psll->length();
			THEN("The value should be 50 and length should be 149") {
				REQUIRE(value == 50);
				REQUIRE(length == 149);
			}
		}
		
		WHEN("Inserting 255 at pos 50") {
			psll->insert(255, 50);
			int value = psll->item_at(50);
			size_t length = psll->length();
			THEN("The value at pos 50 should be 255 and the length should be 151") {
				REQUIRE(value == 255);
				REQUIRE(length == 151);
			}
		}
		
		WHEN("Clearing the list") {
			psll->clear();
			bool x = psll->is_empty();
			size_t length = psll->length();
			THEN("Length should be 0 and is_empty() should be true") {
				REQUIRE(x == true);
				REQUIRE(length == 0);
			}
		}
		
		WHEN("Checking contains for 177") {
			bool x = psll->contains(177,equals_function);
		}
		
		WHEN("Popping back 50 times") {
			for (int i = 0; i < 50; i++) {
				psll->pop_back();
			}
			size_t pool_length = psll->pool_length();
			THEN("Pool length should be 50") {
				REQUIRE(pool_length == 50);
			}
		}
		
		WHEN("Popping back 50 times and pushing back 25 times") {
			for (int i = 0; i < 50; i++) {
				psll->pop_back();
			}
			for (int i = 0; i < 25; i++) {
				psll->push_back(i);
			}
			size_t pool_length = psll->pool_length();
			THEN("Pool length should be 25") {
				REQUIRE(pool_length == 25);
			}
		}
		
		delete psll;
	} 
}

SCENARIO ("Testing constant-ness(?)") {
	GIVEN ("A constant list") {
		PSLL<int> * psll = new PSLL<int>;
		
		psll->push_back(3);
		psll->push_front(1);
		psll->insert(0,0);
		psll->insert(2,2);
		psll->print(std::cout);
		
		const PSLL<int> * psll2 = psll; 
		
		WHEN("Checking its length") {
			size_t size = psll2->length();
			THEN ("Length should be returned as 4") {
				REQUIRE(size == 4);
			}
		}
		
		WHEN("Checking the first item") {
			int value = psll2->peek_front();
			THEN("The value should be 0") {
				REQUIRE(value == 0);
			}
		}
		
		WHEN("Testing iterators") {
			PSLL<int>::const_iterator iter = psll2->begin();
			PSLL<int>::const_iterator end = psll2->end();
			THEN("Iterator should return list") {
				size_t array[4] = {0,1,2,3};
				for (int i = 0; iter!= end; ++iter, ++i) {
					REQUIRE(*iter == array[i]);
				}
			}
		}
		
		delete psll;
	}
}


