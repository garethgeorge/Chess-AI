#ifndef __BOARD_H_
#define __BOARD_H_

#include <cstring>
#include <string>
#include <iostream>
#include <climits>


// general constants that don't change
const int BOARD_SIDE = 8;
const int BOARD_SIZE = BOARD_SIDE * BOARD_SIDE;

// pieces (note that changing these constants may break the piece value lookup table in board.cpp)
typedef signed char Piece;
const Piece PIECE_PAWN = 1;
const Piece PIECE_KNIGHT = 2;
const Piece PIECE_BISHOP = 3;
const Piece PIECE_ROOK = 4;
const Piece PIECE_QUEEN = 5;
const Piece PIECE_KING = 6;

// players
typedef signed int Player;
const Player PLAYER_WHITE =  1;
const Player PLAYER_BLACK = -1;


class Move {
friend class MoveApplicator;
private:
	// indeces
	signed char a; // always >= 0
	signed char b; // always >= 0
	signed char c; // meh.
	signed char d; // a -> b 
public:
	Move() { }
	Move(signed char startPos, signed char endPos)
		: a(startPos), b(endPos), d(-1) { };
	Move(signed char startPos, signed char endPos, signed char newPiece) 
		: a(startPos), b(endPos), c(newPiece), d(-2) { };
	Move(signed char startPos, signed char endPos, signed char startPos2, signed char endPos2) 
		: a(startPos), b(endPos), c(startPos2), d(endPos2) { };

	inline Move& operator=(const Move& other) {
		memcpy(this, &other, sizeof(Move));
		return *this;
	}

	std::string toString() const;
};


class Board {
friend class MoveApplicator;
private:
	Piece board[BOARD_SIZE];
public:
	Board();

	class MoveIterator {
	private:
		Player player;
		Board* board;
		int index;

		int movesInQueue;
		Move moveQueue[BOARD_SIDE * 4]; // pleanty of extra space!

		// utilities for generating moves
		inline void addMoveToQueue(const Move& m) {
			moveQueue[movesInQueue] = m;
			movesInQueue++;
		}

		template<int dx, int dy>
		inline void stepInDirection(int x, int y) {
			const int indexStart = Board::posToIndex(x, y);
			x += dx;
			y += dy;
			while (x >= 0 && y >= 0 && y < BOARD_SIDE && x < BOARD_SIDE) {
				if (!moveTo(indexStart, Board::posToIndex(x, y)))
					break ;
				x += dx;
				y += dy;
			}
		}

		// returns false if it should stop moving either
		// because the path was blocked by a piece
		inline bool moveTo(int from, int to) {
			if (board->board[to] != 0) {
				if (board->board[to] * player < 0)
					addMoveToQueue(Move(from, to));
				return false;
			} 
			addMoveToQueue(Move(from, to));
			return true;
		}


		template<int dx, int dy>
		inline bool moveToOffset(int index, int x, int y) {
			if (dx > 0) {
				if (x + dx >= BOARD_SIDE)
					return false;
			}
			else if (dx < 0) {
				if (x + dx < 0)
					return false;
			}
			if (dy > 0) {
				if (y + dy >= BOARD_SIDE)
					return false;
			}
			else if (dy < 0) {
				if (y + dy < 0)
					return false;
			}
			return moveTo(index, Board::posToIndex(x + dx, y + dy));
		}

		template<int dx, int dy>
		inline bool moveToOffsetNoCapture(int index, int x, int y) {
			if (dx > 0) {
				if (x + dx >= BOARD_SIDE)
					return false;
			}
			else if (dx < 0) {
				if (x + dx < 0)
					return false;
			}
			if (dy > 0) {
				if (y + dy >= BOARD_SIDE)
					return false;
			}
			else if (dy < 0) {
				if (y + dy < 0)
					return false;
			}
			if (board->board[Board::posToIndex(x + dx, y + dy)] == 0) {
				addMoveToQueue(Move(index, Board::posToIndex(x + dx, y + dy)));
				return true;
			}
			return false;
		}

		template<int dx, int dy>
		inline bool moveToOffsetIfCanCapture(int index, int x, int y) {
			if (dx > 0) {
				if (x + dx >= BOARD_SIDE)
					return false;
			}
			else if (dx < 0) {
				if (x + dx < 0)
					return false;
			}
			if (dy > 0) {
				if (y + dy >= BOARD_SIDE)
					return false;
			}
			else if (dy < 0) {
				if (y + dy < 0)
					return false;
			}
			if (board->board[Board::posToIndex(x + dx, y + dy)] * player < 0) {
				addMoveToQueue(Move(index, Board::posToIndex(x + dx, y + dy)));
				return true;
			}
			return false;
		}

	public:
		MoveIterator(Board* board, Player player) : 
			board(board), player(player), index(BOARD_SIZE), movesInQueue(0) { }

		void genMovesForPieceAtIndex(int index);

		inline bool getNext(Move& move) {
			while (movesInQueue == 0) {
				index--;
				if (index < 0)
					return false;
				
				if (board->board[index] * player > 0)
					genMovesForPieceAtIndex(index);
			}

			move = moveQueue[--movesInQueue];
			return true;
		}
	};

	void setPiece(int index, Piece p) {
		board[index] = p;
	}

	void empty();

	void print() const;

	int getScore(Player perspective) const;
	int minimax(const Player me, const Player activeTurn, Move& bestMove, int alpha, int beta, int depth);

	static inline int indexGetX(int index) {
		return index % BOARD_SIDE;
	}
	static inline int indexGetY(int index) {
		return index / BOARD_SIDE;
	}

	static inline int posToIndex(int x, int y) {
		return y * BOARD_SIDE + x;
	}

	static inline bool posIsOnBoard(int x, int y) {
		return x >= 0 && x < BOARD_SIDE && y >= 0 && y <= BOARD_SIDE;
	}
};


class MoveApplicator {
private:
	Piece* pieces;
	Piece a;
	Piece b;
public:
	MoveApplicator(Board* b) : pieces(b->board) {}

	inline void apply(const Move& m) {
		if (m.d == -1) {
			a = pieces[m.a]; 
			b = pieces[m.b];

			pieces[m.b] = pieces[m.a];
			pieces[m.a] = 0;
		} else if (m.d == -2) {
			// it is a move with a piece changing type
			a = pieces[m.a];
			b = pieces[m.b];

			pieces[m.b] = m.c;
			pieces[m.a] = 0;
		} else if (m.d >= 0) {
			// it is a normal move
			a = pieces[m.b]; // only need to remember the first piece

			pieces[m.b] = pieces[m.a];
			pieces[m.a] = 0;

			b = pieces[m.c]; // only need to remember the first piece

			pieces[m.d] = pieces[m.c];
			pieces[m.c] = 0;
		}
	}

	inline void revert(const Move& m) {
		if (m.d == -1) {
			pieces[m.a] = a;
			pieces[m.b] = b;
		} else if (m.d == -2) {
			pieces[m.a] = a;
			pieces[m.b] = b;
		} else if (m.d >= 0) {
			pieces[m.a] = pieces[m.b];
			pieces[m.b] = a;
			pieces[m.c] = pieces[m.d];
			pieces[m.d] = b;
		}
	}
};

#endif
