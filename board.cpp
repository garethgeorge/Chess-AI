#include "board.h"
#include <sstream>
#include "include/termcolor.hpp"
#include <iostream>


Board::Board() {
	for (int i = BOARD_SIZE; i >= 0; --i) {
		this->board[i] = 0;
	}


	for (int i = 0; i < BOARD_SIDE; ++i) {
		this->board[BOARD_SIDE * 1 + i] =  PIECE_PAWN;
		this->board[BOARD_SIDE * 6 + i] = -PIECE_PAWN;
	}
	this->board[3] = PIECE_QUEEN;
	this->board[4] = PIECE_KING;
	this->board[0] = this->board[7] = PIECE_ROOK;
	this->board[1] = this->board[6] = PIECE_KNIGHT;
	this->board[2] = this->board[5] = PIECE_BISHOP;

	const int blackOffset = BOARD_SIDE * 7;
	this->board[blackOffset + 0] = this->board[blackOffset + 7] = -PIECE_ROOK;
	this->board[blackOffset + 1] = this->board[blackOffset + 6] = -PIECE_KNIGHT;
	this->board[blackOffset + 2] = this->board[blackOffset + 5] = -PIECE_BISHOP;
	this->board[blackOffset + 3] = -PIECE_QUEEN;
	this->board[blackOffset + 4] = -PIECE_KING;
};

void Board::MoveIterator::genMovesForPieceAtIndex(Player player, int index) {
	Piece* pieces = board->board;
	Piece p = pieces[index];

	int x = Board::indexGetX(index);
	int y = Board::indexGetY(index);

	switch (p) {
		case PIECE_PAWN:
			
			if (y + player >= 0 && y + player < BOARD_SIDE && pieces[Board::posToIndex(x, y + player)]) {
				addMoveToQueue(Move(index, Board::posToIndex(x, y + player)));
			}

			if (y + player * 2 >= 0 && y + player * 2 < BOARD_SIDE && pieces[Board::posToIndex(x, y + player * 2)]) {
				addMoveToQueue(Move(index, Board::posToIndex(x, y + player * 2)));

				if (Board::posIsOnBoard(x + 1, y + player * 2)) {
					addMoveToQueue(Move(index, Board::posToIndex(x + 1, y + player * 2)));
				}

				if (Board::posIsOnBoard(x - 1, y + player * 2)) {
					addMoveToQueue(Move(index, Board::posToIndex(x - 1, y + player * 2)));
				}
			}

			break ;
		case PIECE_BISHOP: 

			break ;
		case PIECE_KNIGHT:

			break ;
		case PIECE_ROOK:

			break ;
		case PIECE_QUEEN:

			break ;
		case PIECE_KING:

			break ;
	}
}

void Board::print() {
	auto& ss = std::cout;
	for (int y = 0; y < BOARD_SIDE; ++y) {
		ss << termcolor::bold;
		for (int x = 0; x < BOARD_SIDE; ++x) {
			int i = y * BOARD_SIDE + x;
			Piece p = this->board[i];
			if ((i + i / 8) % 2 == 0)
				ss << termcolor::on_grey;
			else
				ss << termcolor::on_cyan;
			if (p != 0) {
				if (p < 0) {
					p = -p;
					ss << termcolor::red;
				} else 
					ss << termcolor::white;

				wchar_t c;
				switch (p) {
					case PIECE_PAWN:
						ss << "P";
						break;
					case PIECE_BISHOP:
						ss << "B";
						break;
					case PIECE_ROOK:
						ss << "R";
						break ;
					case PIECE_KNIGHT:
						ss << "N";
						break;
					case PIECE_KING:
						ss << "K";
						break;
					case PIECE_QUEEN:
						ss << "Q";
						break;
				}
			} else
				ss << " ";
		}
		ss << termcolor::reset << std::endl;
	}
}