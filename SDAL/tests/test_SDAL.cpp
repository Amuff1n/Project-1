#define CATCH_CONFIG_MAIN
#include "../catch.hpp"

#include "../SDAL.h"

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
		
		List<int> * sdal = new SDAL<int>(5);
		
		sdal->push_back(3);
		sdal->push_front(1);
		sdal->insert(0,0);
		sdal->insert(2,2);
		sdal->print(std::cout);
		
		WHEN ("Length checked") {
			size_t length = sdal->length();
			THEN ("Length should be 4") {
				REQUIRE(length == 4);
			}
		}
		
		WHEN("Checking emptiness") {
			bool empty = sdal->is_empty();
			THEN("The list should not be empty") {
				REQUIRE(empty == false);
			}
		}
		
		WHEN("Peeking front") {
			size_t front = sdal->peek_front();
			THEN ("Front should be 0") {
				REQUIRE(front == 0);
			}
		}
		
		WHEN("Peeking back") {
			size_t back = sdal->peek_back();
			THEN ("Back should be 3") {
				REQUIRE(back == 3);
			}
		}
		
		WHEN("Peeking at position 1") {
			size_t item = sdal->item_at(1);
			THEN("Item should 1") {
				REQUIRE(item == 1);
			}
		}
		
		WHEN("Returning contents of list as array") {
			int * temp_array = sdal->contents();
			size_t length = sdal->length();
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
			sdal->replace(4,2);
			size_t item = sdal->item_at(2);
			THEN ("Item at position 2 should be 4") {
				REQUIRE(item == 4);
			}
		}
		
		WHEN("Removing position 0") {
			size_t item = sdal->remove(0);
			THEN("The value should be 0") {
				REQUIRE(item == 0);
			}
		}
		
		WHEN("Popping front") {
			size_t front = sdal->pop_front();
			THEN("The front should be 1") {
				REQUIRE(front == 0);
			}
		}
		
		WHEN("Popping back") {
			size_t back = sdal->pop_back();
			THEN("The back should be 3") {
				REQUIRE(back == 3);
			}
		}
		
		WHEN("Clearing the list") {
			sdal->clear();
			size_t length = sdal->length();
			THEN("The length should be 0") {
				REQUIRE(length == 0);
			}
		}
		
		WHEN("Checking if the list contains the value 3") {
			bool contains = sdal->contains(3,equals_function);
			THEN("Should return true") {
				REQUIRE(contains == true);
			}
		}
		
		//clean up
		delete sdal;
		
	}
}

SCENARIO("Testing 'big five' and iterators") {
	GIVEN ("List of integers") {
		
		//declaring SDAL here, not List
		//TODO put virtual iterator methods (and maybe virtual 'big five') in List.h
		SDAL<int> sdal(5);
		
		sdal.push_back(3);
		sdal.push_front(1);
		sdal.insert(0,0);
		sdal.insert(2,2);
		sdal.print(std::cout);
		
		WHEN("Testing iterators") {
			SDAL<int>::iterator iter = sdal.begin();
			SDAL<int>::iterator end = sdal.end();
			THEN("Iterator should return list") {
				size_t array[4] = {0,1,2,3};
				for (int i = 0; iter != end; ++iter, ++i) {
					REQUIRE(*iter == array[i]);
				}
			}
		}
		
		WHEN("Testing copy constructor") {
			SDAL<int> sdal2(sdal);
			THEN("List should be same for new copy") {
				for (int i = 0; i < sdal2.length(); i++) {
					REQUIRE(sdal2.item_at(i) == sdal.item_at(i));
				}
			}
		}
		
		WHEN("Testing copy assignment") {
			SDAL<int> sdal2;
			sdal2.push_back(24);
			sdal2.push_front(23);
			
			sdal2 = sdal;
			
			THEN("List should be same for new copy") {
				for (int i = 0; i < sdal2.length(); i++) {
					REQUIRE(sdal2.item_at(i) == sdal.item_at(i));
				}
			}
		}

		WHEN("Testing move constructor") {
			SDAL<int>  sdal2(std::move(sdal));
			THEN("List should move to new copy") {
				size_t array[4] = {0,1,2,3};
				for (int i = 0; i < sdal2.length(); i++) {
					REQUIRE(array[i] == sdal2.item_at(i));
				}
			}
		}
		
		WHEN("Testing move assignment") {
			SDAL<int> sdal2;
			sdal2.push_back(24);
			sdal2.push_front(23);
			
			sdal2 = std::move(sdal);
			THEN("List should move to new copy") {
				size_t array[4] = {0,1,2,3};
				for (int i = 0; i < sdal2.length(); i++) {
					REQUIRE(array[i] == sdal2.item_at(i));
				}
			}
		}
	}
}

SCENARIO("Testing large lists") {
	GIVEN("A list of 150 ints") {
		List<int> * sdal = new SDAL<int>(5);
		for (int i = 0; i < 150; i++) {
			sdal->push_back(i);
		}
		
		WHEN("Checking length") {
			size_t length = sdal->length();
			THEN("Length should be 150") {
				REQUIRE(length == 150);
			}
		}
		
		WHEN("Popping front") {
			int value = sdal->pop_front();
			size_t length = sdal->length();
			THEN("The value should be 0 and the length should be 149") {
				REQUIRE(value == 0);
				REQUIRE(length == 149);
			}	
		}
		
		WHEN("Popping back") {
			int value = sdal->pop_back();
			size_t length = sdal->length();
			THEN("The value should be 149 and the length should be 149") {
				REQUIRE(value == 149);
				REQUIRE(length == 149);
			}	
		}
		
		WHEN("Removing int from pos 50") {
			int value = sdal->remove(50);
			size_t length = sdal->length();
			THEN("The value should be 50 and length should be 149") {
				REQUIRE(value == 50);
				REQUIRE(length == 149);
			}
		}
		
		WHEN("Inserting 255 at pos 50") {
			sdal->insert(255, 50);
			int value = sdal->item_at(50);
			size_t length = sdal->length();
			THEN("The value at pos 50 should be 255 and the length should be 151") {
				REQUIRE(value == 255);
				REQUIRE(length == 151);
			}
		}
		
		WHEN("Clearing the list") {
			sdal->clear();
			bool x = sdal->is_empty();
			size_t length = sdal->length();
			THEN("Length should be 0 and is_empty() should be true") {
				REQUIRE(x == true);
				REQUIRE(length == 0);
			}
		}
		
		WHEN("Checking contains for 177") {
			bool x = sdal->contains(177,equals_function);
		}
		
		delete sdal;
	} 
}


SCENARIO ("Testing constant-ness(?)") {
	GIVEN ("A constant list") {
		SDAL<int> * sdal = new SDAL<int>(5);
		
		sdal->push_back(3);
		sdal->push_front(1);
		sdal->insert(0,0);
		sdal->insert(2,2);
		sdal->print(std::cout);
		
		const SDAL<int> * sdal2 = sdal; 
		
		WHEN("Checking its length") {
			size_t size = sdal2->length();
			THEN ("Length should be returned as 4") {
				REQUIRE(size == 4);
			}
		}
		
		WHEN("Checking the first item") {
			int value = sdal2->peek_front();
			THEN("The value should be 0") {
				REQUIRE(value == 0);
			}
		}

		WHEN("Testing iterators") {
			SDAL<int>::const_iterator iter = sdal2->begin();
			SDAL<int>::const_iterator end = sdal2->end();
			THEN("Iterator should return list") {
				size_t array[4] = {0,1,2,3};
				for (int i = 0; iter != end; ++iter, ++i) {
					REQUIRE(*iter == array[i]);
				}
			}
		}
		
		delete sdal;
	}
}

