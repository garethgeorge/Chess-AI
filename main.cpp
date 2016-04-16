#include "board.h"
#include <iostream>
#include "include/termcolor.hpp"

int main() {
	std::cout << "Chess engine by Gareth George" << std::endl;
	std::cout << "\tnow with beautiful colorful chess boards!" << std::endl;	
	Board* b = new Board;
	b->print();
	delete b;

	

	return 0;
}