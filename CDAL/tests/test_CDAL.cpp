#define CATCH_CONFIG_MAIN
#include "../catch.hpp"

#include "../CDAL.h"

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
		
		CDAL<int> * cdal = new CDAL<int>;
		
		cdal->push_back(3);
		cdal->push_front(1);
		cdal->insert(0,0);
		cdal->insert(2,2);
		cdal->print(std::cout);
		
		WHEN ("Length checked") {
			size_t length = cdal->length();
			THEN ("Length should be 4") {
				REQUIRE(length == 4);
			}
		}
		
		WHEN("Checking emptiness") {
			bool empty = cdal->is_empty();
			THEN("The list should not be empty") {
				REQUIRE(empty == false);
			}
		}
		
		WHEN("Peeking front") {
			size_t front = cdal->peek_front();
			THEN ("Front should be 0") {
				REQUIRE(front == 0);
			}
		}
		
		WHEN("Peeking back") {
			size_t back = cdal->peek_back();
			THEN ("Back should be 3") {
				REQUIRE(back == 3);
			}
		}
		
		WHEN("Peeking at position 1") {
			size_t item = cdal->item_at(1);
			THEN("Item should 1") {
				REQUIRE(item == 1);
			}
		}
		
		WHEN("Returning contents of list as array") {
			int * temp_array = cdal->contents();
			size_t length = cdal->length();
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
			cdal->replace(4,2);
			size_t item = cdal->item_at(2);
			THEN ("Item at position 2 should be 4") {
				REQUIRE(item == 4);
			}
		}
		
		WHEN("Removing position 0") {
			size_t item = cdal->remove(0);
			THEN("The value should be 0") {
				REQUIRE(item == 0);
			}
		}
		
		WHEN("Popping front") {
			size_t front = cdal->pop_front();
			THEN("The front should be 1") {
				REQUIRE(front == 0);
			}
		}
		
		WHEN("Popping back") {
			size_t back = cdal->pop_back();
			THEN("The back should be 3") {
				REQUIRE(back == 3);
			}
		}
		
		WHEN("Clearing the list") {
			cdal->clear();
			size_t length = cdal->length();
			THEN("The length should be 0") {
				REQUIRE(length == 0);
			}
		}
		
		WHEN("Checking if the list contains the value 3") {
			bool contains = cdal->contains(3,equals_function);
			THEN("Should return true") {
				REQUIRE(contains == true);
			}
		}
		
		//clean up
		delete cdal;
		
	}
}

SCENARIO("Testing 'big five' and iterators") {
	GIVEN ("List of integers") {
		
		//declaring CDAL here, not List
		//TODO put virtual iterator methods (and maybe virtual 'big five') in List.h
		CDAL<int> cdal;
		
		cdal.push_back(3);
		cdal.push_front(1);
		cdal.insert(0,0);
		cdal.insert(2,2);
		cdal.print(std::cout);
		
		WHEN("Testing iterators") {
			CDAL<int>::iterator iter = cdal.begin();
			CDAL<int>::iterator end = cdal.end();
			THEN("Iterator should return list") {
				size_t array[4] = {0,1,2,3};
				for (int i = 0; iter != end; ++iter, ++i) {
					REQUIRE(*iter == array[i]);
				}
			}
		}
		
		WHEN("Testing copy constructor") {
			CDAL<int> cdal2(cdal);
			THEN("List should be same for new copy") {
				for (int i = 0; i < cdal2.length(); i++) {
					REQUIRE(cdal2.item_at(i) == cdal.item_at(i));
				}
			}
		}
	
		WHEN("Testing copy assignment") {
			CDAL<int> cdal2;
			cdal2.push_back(24);
			cdal2.push_front(23);
			
			cdal2 = cdal;
			
			THEN("List should be same for new copy") {
				for (int i = 0; i < cdal2.length(); i++) {
					REQUIRE(cdal2.item_at(i) == cdal.item_at(i));
				}
			}
		}
		
		WHEN("Testing move constructor") {
			CDAL<int> cdal2(std::move(cdal));
			THEN("List should move to new copy") {
				size_t array[4] = {0,1,2,3};
				for (int i = 0; i < cdal2.length(); i++) {
					REQUIRE(array[i] == cdal2.item_at(i));
				}
			}
		}
		
		WHEN("Testing move assignment") {
			CDAL<int> cdal2;
			cdal2.push_back(24);
			cdal2.push_front(23);
			
			cdal2 = std::move(cdal);
			THEN("List should move to new copy") {
				size_t array[4] = {0,1,2,3};
				for (int i = 0; i < cdal2.length(); i++) {
					REQUIRE(array[i] == cdal2.item_at(i));
				}
			}
		}
	}
}

SCENARIO("Testing large lists") {
	GIVEN("A list of 150 ints") {
		List<int> * cdal = new CDAL<int>;
		for (int i = 0; i < 150; i++) {
			cdal->push_back(i);
		}
		
		WHEN("Checking length") {
			size_t length = cdal->length();
			THEN("Length should be 150") {
				REQUIRE(length == 150);
			}
		}
		
		WHEN("Popping front") {
			int value = cdal->pop_front();
			size_t length = cdal->length();
			THEN("The value should be 0 and the length should be 149") {
				REQUIRE(value == 0);
				REQUIRE(length == 149);
			}	
		}
		
		WHEN("Popping back") {
			int value = cdal->pop_back();
			size_t length = cdal->length();
			THEN("The value should be 149 and the length should be 149") {
				REQUIRE(value == 149);
				REQUIRE(length == 149);
			}	
		}
		
		WHEN("Removing int from pos 50") {
			int value = cdal->remove(50);
			size_t length = cdal->length();
			THEN("The value should be 50 and length should be 149") {
				REQUIRE(value == 50);
				REQUIRE(length == 149);
			}
		}
		
		WHEN("Inserting 255 at pos 50") {
			cdal->insert(255, 50);
			int value = cdal->item_at(50);
			size_t length = cdal->length();
			THEN("The value at pos 50 should be 255 and the length should be 151") {
				REQUIRE(value == 255);
				REQUIRE(length == 151);
			}
		}
		
		WHEN("Clearing the list") {
			cdal->clear();
			bool x = cdal->is_empty();
			size_t length = cdal->length();
			THEN("Length should be 0 and is_empty() should be true") {
				REQUIRE(x == true);
				REQUIRE(length == 0);
			}
		}
		
		WHEN("Checking contains for 177") {
			bool x = cdal->contains(77,equals_function);
			THEN("It should exist") {
				REQUIRE(x == true);
			}
		}
		delete cdal;
	} 
}

SCENARIO ("Testing constant-ness(?)") {
	GIVEN ("A constant list") {
		CDAL<int> * cdal = new CDAL<int>;
		
		cdal->push_back(3);
		cdal->push_front(1);
		cdal->insert(0,0);
		cdal->insert(2,2);
		cdal->print(std::cout);
		
		const CDAL<int> * cdal2 = cdal; 
		
		WHEN("Checking its length") {
			size_t size = cdal2->length();
			THEN ("Length should be returned as 4") {
				REQUIRE(size == 4);
			}
		}
		
		WHEN("Checking the first item") {
			int value = cdal2->peek_front();
			THEN("The value should be 0") {
				REQUIRE(value == 0);
			}
		}
		
		WHEN("Testing iterators") {
			CDAL<int>::const_iterator iter = cdal2->begin();
			CDAL<int>::const_iterator end = cdal2->end();
			THEN("Iterator should return list") {
				size_t array[4] = {0,1,2,3};
				for (int i = 0; iter != end; ++iter, ++i) {
					REQUIRE(*iter == array[i]);
				}
			}
		}
		
		delete cdal;
	}
}


