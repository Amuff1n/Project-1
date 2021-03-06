#include <iostream>
#include "CBL.h"
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

int main() {
	//testing CBL, int type
	int choice, input, pos;
	
	std::cout<<"Input an initial size for the array-based list: "<<std::endl;
	std::cin>>input;
	CBL<int> cbl_1(input);
	
	std::cout<<"Be aware, positions start at 0!"<<std::endl;
	
	while (true) {
		std::cout<<"\n---Operations on CBL---"<<std::endl;
		std::cout<<"1. Push front"<<std::endl;
        std::cout<<"2. Push back"<<std::endl;
        std::cout<<"3. Insert at position"<<std::endl;
        std::cout<<"4. Replace at position"<<std::endl;
        std::cout<<"5. Remove at position"<<std::endl;
        std::cout<<"6. Pop back"<<std::endl;
        std::cout<<"7. Pop front"<<std::endl;
        std::cout<<"8. Peek at position (item_at)"<<std::endl;
        std::cout<<"9. Peek back "<<std::endl;
        std::cout<<"10. Peek front "<<std::endl;
		std::cout<<"11. Check is_empty? "<<std::endl;
		std::cout<<"12. Check is_full? "<<std::endl;
		std::cout<<"13. Check length"<<std::endl;
		std::cout<<"14. Clear the list "<<std::endl;
		std::cout<<"15. List contains input?"<<std::endl;
		std::cout<<"16. Print list"<<std::endl;
		std::cout<<"17. Return array of contents"<<std::endl;
		std::cout<<"18. Test Iterator (List must have 1+ elements!)"<<std::endl;
		std::cout<<"19. Test Copy member functions: "<<std::endl;
		std::cout<<"20. Exit"<<std::endl;
        std::cout<<"Enter your choice : ";
        std::cin>>choice;
        switch(choice)
        {
        case 1:
            std::cout<<"Input a value to push to front: "<<std::endl;
            std::cin>>input;
			cbl_1.push_front(input);
            break;
        case 2:
            std::cout<<"Input a value to push to back: "<<std::endl;
			std::cin>>input;
			cbl_1.push_back(input);
            break;
        case 3:
            std::cout<<"Input a value to insert: "<<std::endl;
			std::cin>>input;
			std::cout<<"Input a position to insert at: "<<std::endl;
			std::cin>>pos;
			cbl_1.insert(input,pos);
            break;
        case 4:
            std::cout<<"Input a value to replace: "<<std::endl;
			std::cin>>input;
			std::cout<<"Input a position to replace at: "<<std::endl;
			std::cin>>pos;
			cbl_1.replace(input,pos);
            break;
        case 5:
            std::cout<<"Input a position to remove node: "<<std::endl;
			std::cin>>pos;
			std::cout<<"Removing and returning value at pos: "<<std::endl;
			std::cout<<cbl_1.remove(pos);
            break;
        case 6:
            std::cout<<"Popping back: "<<std::endl;  
			std::cout<<cbl_1.pop_back()<<std::endl;
            break;
        case 7:
            std::cout<<"Popping front: "<<std::endl;
			std::cout<<cbl_1.pop_front()<<std::endl;
            break;
        case 8:
            std::cout<<"Input a position to peek at: "<<std::endl;
			std::cin>>pos;
            std::cout<<cbl_1.item_at(pos)<<std::endl;
            break;
        case 9:
            std::cout<<"Peeking back: "<<std::endl;
			std::cout<<cbl_1.peek_back()<<std::endl;
            break;
        case 10:
            std::cout<<"Peeking front: "<<std::endl;
			std::cout<<cbl_1.peek_front()<<std::endl;
            break;
		case 11:
            std::cout<<"Checking if list is empty: "<<std::endl;
			cbl_1.is_empty();
            break; 
		case 12:
            std::cout<<"Checking if list is full: "<<std::endl;
			cbl_1.is_full();
            break; 
		case 13:
            std::cout<<"Getting length of list: "<<std::endl;
			std::cout<<cbl_1.length()<<std::endl;
            break; 
		case 14:
            std::cout<<"Clearing the list: "<<std::endl;
			cbl_1.clear();
            break;
		case 15: 
            std::cout<<"Input a value to check for: "<<std::endl;
			std::cin>>input;
			cbl_1.contains(input, equals_function);
            break; 	
		case 16:
            std::cout<<"Printing list: "<<std::endl;
			cbl_1.print(std::cout);
            break; 	
		case 17:{
            std::cout<<"Copying list to array and returning: "<<std::endl;
			int * output = cbl_1.contents();
			delete[] output;
            break;
		}
		case 18: {
			std::cout<<"Testing Iterator by printing out contents of list: "<<std::endl;
			CBL<int>::iterator iter = cbl_1.begin();
			CBL<int>::iterator end = cbl_1.end();
			std::cout<<"[ ";
			for ( ; iter != end; ++iter) {
				std::cout<<*iter<< ", ";
			}
			std::cout<<"]"<<std::endl;
			break;
		}
		case 19: {
			std::cout<<"Testing copy constructor: "<<std::endl;
			CBL<int> cbl_2 = cbl_1;
			cbl_2.print(std::cout);
			std::cout<<"Testing copy assignment: "<<std::endl;
			CBL<int> cbl_3(5); //init the dynamic array to some random size for testing purposes
			cbl_3.push_back(1337); //throw some value in there to make sure we don't get it back
			cbl_3.print(std::cout); //print that to show it is different
			cbl_3 = cbl_1;
			cbl_3.print(std::cout); //after copy assignment, show that array is updated
			break;
		}
		case 20:
			std::cout<<"Exiting"<<std::endl;
			return 0;
        default:
            std::cout<<"Invalid Choice"<<std::endl;
        }
	}
	return 0;
}