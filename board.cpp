#include "board.h"
#include <sstream>
#include "include/termcolor.hpp"
#include <iostream>

// get the value of a piece
int pieceValues[] = {-1, 1, 3, 3, 5, 9, 1000};
inline int getPieceValue(Piece p) {
	if (p < 0)
		return -pieceValues[-p];
	else
		return pieceValues[p];
}

// get the letter for the piece
char pieceToChar(Piece p) {
	switch (p) {
		case PIECE_PAWN:
			return 'P';
		case PIECE_BISHOP:
			return 'B';
		case PIECE_ROOK:
			return 'R';
		case PIECE_KNIGHT:
			return 'N';
		case PIECE_KING:
			return 'K';
		case PIECE_QUEEN:
			return 'Q';
	}
	return '-';
}

std::string Move::toString() const {
	std::stringstream ss;
	if (d >= 0) {
		ss << "(" << (int) a << "->" << (int) b << ", " << (int) c << "-> " << (int) d << ")";
	} else if (d == -1) {
		ss << "([" << Board::indexGetX(a) << "," << Board::indexGetY(a) << "]" << "->[" << Board::indexGetX(b) << "," << Board::indexGetY(b) << "])";
	} else if (d == -2) {
		ss << "([" << Board::indexGetX(a) << ", " << Board::indexGetY(a) << "]->" << (int) b << " AS " << pieceToChar(c) << ")";
	}
	return ss.str();
}


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

void Board::empty() {
	for (int i = 0; i < BOARD_SIZE; ++i)
		this->board[i] = 0;
}


void Board::MoveIterator::genMovesForPieceAtIndex(int index) {

	Piece* pieces = board->board;
	Piece p = pieces[index] * player;

	int x = Board::indexGetX(index);
	int y = Board::indexGetY(index);

	switch (p) {
		case PIECE_PAWN:

			if (player == 1) {
				if (moveToOffsetNoCapture<0,1>(index, x, y) && y == 1) {
					moveToOffsetNoCapture<0,2>(index, x, y);
				}
				
				moveToOffsetIfCanCapture<-1,1>(index, x, y);
				moveToOffsetIfCanCapture<1,1>(index, x, y);
			} else if (player == -1) {
				if (moveToOffsetNoCapture<0,-1>(index, x, y) && y == 6) {
					moveToOffsetNoCapture<0,-2>(index, x, y);
				}
				moveToOffsetIfCanCapture<-1,-1>(index, x, y);
				moveToOffsetIfCanCapture<1,-1>(index, x, y);
			}

			/* TODO: implement en passant rules */
			
			if (y + player >= 0 && y + player < BOARD_SIZE && pieces[Board::posToIndex(x, y + player)] == 0) {
				addMoveToQueue(Move(index, Board::posToIndex(x, y + player)));
			}

			if (((player == 1 && y == 1) || (player == -1 && y == 6))  && pieces[Board::posToIndex(x, y + player * 2)] == 0) {
				addMoveToQueue(Move(index, Board::posToIndex(x, y + player * 2)));

				if (Board::posIsOnBoard(x + 1, y + player * 2) && pieces[Board::posToIndex(x + 1, player * 2)] * player < 0) {
					addMoveToQueue(Move(index, Board::posToIndex(x + 1, y + player * 2)));
				}

				if (Board::posIsOnBoard(x - 1, y + player * 2) && pieces[Board::posToIndex(x - 1, player * 2)] * player < 0) {
					addMoveToQueue(Move(index, Board::posToIndex(x - 1, y + player * 2)));
				}
			}


			if (Board::posIsOnBoard(x + 1, y + player) && pieces[Board::posToIndex(x + 1, player)] * player < 0) {
				addMoveToQueue(Move(index, Board::posToIndex(x + 1, y + player)));
			}

			if (Board::posIsOnBoard(x - 1, y + player) && pieces[Board::posToIndex(x - 1, player)] * player < 0) {
				addMoveToQueue(Move(index, Board::posToIndex(x - 1, y + player)));
			}

			break ;

		case PIECE_KNIGHT:
			
			moveToOffset<2,1>(index, x, y);
			moveToOffset<1,2>(index, x, y);
			
			moveToOffset<2,-1>(index, x, y);
			moveToOffset<1,-2>(index, x, y);
			
			moveToOffset<-2,1>(index, x, y);
			moveToOffset<-1,2>(index, x, y);

			moveToOffset<-2,-1>(index, x, y);
			moveToOffset<-1,-2>(index, x, y);

			break ;
		case PIECE_ROOK:

			stepInDirection<1, 0>(x, y);
			stepInDirection<-1, 0>(x, y);
			stepInDirection<0, 1>(x, y);
			stepInDirection<0, -1>(x, y);

			break ;
		case PIECE_BISHOP:
			
			stepInDirection<1, 1>(x, y);
			stepInDirection<-1, 1>(x, y);
			stepInDirection<1, -1>(x, y);
			stepInDirection<-1, -1>(x, y);

			break;

		case PIECE_QUEEN:

			stepInDirection<1, 0>(x, y);
			stepInDirection<-1, 0>(x, y);
			stepInDirection<0, 1>(x, y);
			stepInDirection<0, -1>(x, y);

			stepInDirection<1, 1>(x, y);
			stepInDirection<-1, 1>(x, y);
			stepInDirection<1, -1>(x, y);
			stepInDirection<-1, -1>(x, y);

			break ;
		case PIECE_KING:

			if (x + 1 < BOARD_SIDE)
				moveTo(index, index + 1);
			if (x - 1 >= 0)
				moveTo(index, index - 1);
			if (y + 1 < BOARD_SIDE)
				moveTo(index, index + BOARD_SIDE);
			if (y - 1 >= 0)
				moveTo(index, index - BOARD_SIDE);

			if (x + 1 < BOARD_SIDE && y + 1 < BOARD_SIDE)
				moveTo(index, index + 1 + BOARD_SIDE);
			if (x + 1 < BOARD_SIDE && y - 1 >= 0)
				moveTo(index, index + 1 - BOARD_SIDE);
			if (x - 1 >= 0 && y + 1 < BOARD_SIDE)
				moveTo(index, index - 1 + BOARD_SIDE);
			if (x - 1 >= 0 && y - 1 >= 0)
				moveTo(index, index - 1 - BOARD_SIDE);

			break ;
	}
}

void Board::print() const {
	auto& ss = std::cout;
	ss << termcolor::reset << " " << termcolor::grey << termcolor::on_white;
	for (int i = 0; i < BOARD_SIDE; ++i) {
		ss << (char) ('a' + i);
	}
	ss << termcolor::reset << std::endl;

	for (int y = 0; y < BOARD_SIDE; ++y) {
		ss << termcolor::on_white << termcolor::grey << (y + 1) << termcolor::bold;
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

				ss << pieceToChar(p);
			} else
				ss << " ";
		}
		ss << termcolor::reset << std::endl;
	}
}

int Board::getScore(Player perspective) const {
	int score = 0;
	
	for (int i = BOARD_SIZE - 1; i >= 0; --i)
		if (board[i] != 0)
			score += getPieceValue(board[i]);

	return score * perspective * 2 + rand() % 2;
}

int Board::minimax(const Player me, const Player activeTurn, Move& bestMove, int alpha, int beta, int depth) {
	if (depth <= 0)
		return getScore(me);

	const Player nextTurn = activeTurn * (-1);

	Move move;
	Move trash;

	Board::MoveIterator mIter(this, activeTurn);
	MoveApplicator applicator(this);

	if (me == activeTurn) {
		while (mIter.getNext(move)) {
			applicator.apply(move);
			int score = minimax(me, nextTurn, trash, alpha, beta, depth - 1);
			applicator.revert(move);

			if (score >= beta) {
				return beta;
			}

			if (score > alpha) {
				alpha = score;
				bestMove = move;
			}
		}

		return alpha;
	} else {
		while (mIter.getNext(move)) {
			applicator.apply(move);
			int score = minimax(me, nextTurn, trash, alpha, beta, depth - 1);
			applicator.revert(move);
			
			if (score <= alpha)
				return alpha;
			if (score < beta) {
				beta = score;
				bestMove = move;
			}
		}

		return beta;
	}

}