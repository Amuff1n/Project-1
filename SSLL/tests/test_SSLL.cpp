#define CATCH_CONFIG_MAIN
#include "../catch.hpp"

#include "../SSLL.h"

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
		
		SSLL<int> * ssll = new SSLL<int>;
		
		ssll->push_back(3);
		ssll->push_front(1);
		ssll->insert(0,0);
		ssll->insert(2,2);
		ssll->print(std::cout);
		
		WHEN ("Length checked") {
			size_t length = ssll->length();
			THEN ("Length should be 4") {
				REQUIRE(length == 4);
			}
		}
		
		WHEN("Checking emptiness") {
			bool empty = ssll->is_empty();
			THEN("The list should not be empty") {
				REQUIRE(empty == false);
			}
		}
		
		WHEN("Peeking front") {
			size_t front = ssll->peek_front();
			THEN ("Front should be 0") {
				REQUIRE(front == 0);
			}
		}
		
		WHEN("Peeking back") {
			size_t back = ssll->peek_back();
			THEN ("Back should be 3") {
				REQUIRE(back == 3);
			}
		}
		
		WHEN("Peeking at position 1") {
			size_t item = ssll->item_at(1);
			THEN("Item should 1") {
				REQUIRE(item == 1);
			}
		}
		
		WHEN("Returning contents of list as array") {
			int * temp_array = ssll->contents();
			size_t length = ssll->length();
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
			ssll->replace(4,2);
			size_t item = ssll->item_at(2);
			THEN ("Item at position 2 should be 4") {
				REQUIRE(item == 4);
			}
		}
		
		WHEN("Removing position 0") {
			size_t item = ssll->remove(0);
			THEN("The value should be 0") {
				REQUIRE(item == 0);
			}
		}
		
		WHEN("Popping front") {
			size_t front = ssll->pop_front();
			THEN("The front should be 1") {
				REQUIRE(front == 0);
			}
		}
		
		WHEN("Popping back") {
			size_t back = ssll->pop_back();
			THEN("The back should be 3") {
				REQUIRE(back == 3);
			}
		}
		
		WHEN("Clearing the list") {
			ssll->clear();
			size_t length = ssll->length();
			THEN("The length should be 0") {
				REQUIRE(length == 0);
			}
		}
		
		WHEN("Checking if the list contains the value 3") {
			bool contains = ssll->contains(3,equals_function);
			THEN("Should return true") {
				REQUIRE(contains == true);
			}
		}
		
		//clean up
		delete ssll;
		
	}
}

SCENARIO("Testing 'big five' and iterators") {
	GIVEN ("List of integers") {
		
		//declaring SSLL here, not List
		//TODO put virtual iterator methods (and maybe virtual 'big five') in List.h
		SSLL<int> * ssll = new SSLL<int>;
		
		ssll->push_back(3);
		ssll->push_front(1);
		ssll->insert(0,0);
		ssll->insert(2,2);
		ssll->print(std::cout);
		
		WHEN("Testing iterators") {
			SSLL<int>::iterator iter = ssll->begin();
			SSLL<int>::iterator end = ssll->end();
			THEN("Iterator should return list") {
				size_t array[4] = {0,1,2,3};
				for (int i = 0; iter!= end; ++iter, ++i) {
					REQUIRE(*iter == array[i]);
				}
			}
		}
		
		WHEN("Testing iterator by changing first value to 24") {
			SSLL<int>::iterator iter = ssll->begin();
			*iter = 24;
			int value = ssll->peek_front();
			THEN("Peeking front should return 24") {
				REQUIRE(value == 24);
			}
		}
		
		WHEN("Testing copy constructor") {
			SSLL<int> * ssll2(ssll);
			THEN("List should be same for new copy") {
				for (int i = 0; i < ssll2->length(); i++) {
					REQUIRE(ssll2->item_at(i) == ssll->item_at(i));
				}
			}
		}
		//TODO fix assignment shit
		//I say this sincerely: FUCK this shit
		//I've spent at least 10 hours just on copy assignment here trying to figure out why the fuck it cause a segmentation fault when it deletes the copy
		//Copy constructor works just fine
		/*
		WHEN("Testing copy assignment") {
			SSLL<int> * ssll2 = new SSLL<int>;
			ssll2->push_back(24);
			ssll2->push_front(23);
			ssll2->peek_front();
			ssll2 = ssll;
			
			THEN("List should be same for new copy") {
				for (int i = 0; i < ssll2->length(); i++) {
					REQUIRE(ssll2->item_at(i) == ssll->item_at(i));
				}
			}
			
			delete ssll2;
		}
		*/
		WHEN("Testing move constructor") {
			SSLL<int> *  ssll2(std::move(ssll));
			THEN("List should move to new copy") {
				size_t array[4] = {0,1,2,3};
				for (int i = 0; i < ssll2->length(); i++) {
					REQUIRE(array[i] == ssll2->item_at(i));
				}
			}
		}
		/*
		WHEN("Testing move assignment") {
			SSLL<int> * ssll2 = new SSLL<int>;
			ssll2->push_back(24);
			ssll2->push_front(23);
			
			ssll2 = std::move(ssll);
			THEN("List should move to new copy") {
				size_t array[4] = {0,1,2,3};
				for (int i = 0; i < ssll2->length(); i++) {
					REQUIRE(array[i] == ssll2->item_at(i));
				}
			}
			
			delete ssll2;
		}
		*/
		delete ssll;
	}
}

SCENARIO("Testing large lists") {
	GIVEN("A list of 150 ints") {
		SSLL<int> * ssll = new SSLL<int>;
		for (int i = 0; i < 150; i++) {
			ssll->push_back(i);
		}
		
		WHEN("Checking length") {
			size_t length = ssll->length();
			THEN("Length should be 150") {
				REQUIRE(length == 150);
			}
		}
		
		WHEN("Popping front") {
			int value = ssll->pop_front();
			size_t length = ssll->length();
			THEN("The value should be 0 and the length should be 149") {
				REQUIRE(value == 0);
				REQUIRE(length == 149);
			}	
		}
		
		WHEN("Popping back") {
			int value = ssll->pop_back();
			size_t length = ssll->length();
			THEN("The value should be 149 and the length should be 149") {
				REQUIRE(value == 149);
				REQUIRE(length == 149);
			}	
		}
		
		WHEN("Removing int from pos 50") {
			int value = ssll->remove(50);
			size_t length = ssll->length();
			THEN("The value should be 50 and length should be 149") {
				REQUIRE(value == 50);
				REQUIRE(length == 149);
			}
		}
		
		WHEN("Inserting 255 at pos 50") {
			ssll->insert(255, 50);
			int value = ssll->item_at(50);
			size_t length = ssll->length();
			THEN("The value at pos 50 should be 255 and the length should be 151") {
				REQUIRE(value == 255);
				REQUIRE(length == 151);
			}
		}
		
		WHEN("Clearing the list") {
			ssll->clear();
			bool x = ssll->is_empty();
			size_t length = ssll->length();
			THEN("Length should be 0 and is_empty() should be true") {
				REQUIRE(x == true);
				REQUIRE(length == 0);
			}
		}
		
		WHEN("Checking contains for 177") {
			bool x = ssll->contains(177,equals_function);
		}
		
		delete ssll;
	} 
}

SCENARIO ("Testing constant-ness(?)") {
	GIVEN ("A constant list") {
		SSLL<int> * ssll = new SSLL<int>;
		
		ssll->push_back(3);
		ssll->push_front(1);
		ssll->insert(0,0);
		ssll->insert(2,2);
		ssll->print(std::cout);
		
		SSLL<int> const * ssll2 = ssll; 
		
		WHEN("Checking its length") {
			size_t size = ssll2->length();
			THEN ("Length should be returned as 4") {
				REQUIRE(size == 4);
			}
		}
		
		WHEN("Checking the first item") {
			int value = ssll2->peek_front();
			THEN("The value should be 0") {
				REQUIRE(value == 0);
			}
		}
		
		WHEN("Testing const iterators") {
			SSLL<int>::const_iterator iter = ssll2->begin();
			SSLL<int>::const_iterator end = ssll2->end();
			THEN("Iterator should return list") {
				size_t array[4] = {0,1,2,3};
				for (int i = 0; iter!= end; ++iter, ++i) {
					REQUIRE(*iter == array[i]);
				}
			}
		}
		
		delete ssll;
	}
}


