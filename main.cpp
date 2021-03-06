#include "board.h"
#include <iostream>
#include <unistd.h>
#include <climits>
#include "include/termcolor.hpp"

/*
class MoveMaker {
private:
	Player p;
public:
	MoveMaker(Player p) : p(p) {

	};

	virtual Move generateMove() {

	};
};


class MoveMakerHuman : public MoveMaker {
public:

	virtual Move generateMove(Player p) : MoveMaker(p) {
		
	}
};

class MoveMakerBot : public MoveMaker {
public:

	virtual Move generateMove(Player p) : MoveMaker(p) {
		
	}

	virtual
};
*/



int main() {
	std::cout << "Chess engine by Gareth George" << std::endl;
	std::cout << "\tnow with beautiful colorful chess boards!" << std::endl;	
	
	Board* board = new Board;
	Move move;
	MoveApplicator ma(board);

	int moveCount = 0;
	while (true) {

		moveCount++;
		board->minimax((Player) 1, 1, move, INT_MIN, INT_MAX, 6);
		std::cout << "moves: " << moveCount << std::endl;
		std::cout << "\tmove: "<< move.toString() << std::endl;
		ma.apply(move);
		board->print();

		moveCount++;
		board->minimax((Player) -1, -1, move, INT_MIN, INT_MAX, 6);
		std::cout << "moves: " << moveCount << std::endl;
		std::cout << "\tmove: "<< move.toString() << std::endl;
		ma.apply(move);
		board->print();

		sleep(2);
	}



	delete board;

	return 0;
}