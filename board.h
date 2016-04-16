#ifndef __BOARD_H_
#define __BOARD_H_

#include <cstring>
#include <string>

// general constants that don't change
const int BOARD_SIDE = 8;
const int BOARD_SIZE = BOARD_SIDE * BOARD_SIDE;

// pieces
typedef signed char Piece;
const Piece PIECE_PAWN = 1;
const Piece PIECE_KNIGHT = 2;
const Piece PIECE_BISHOP = 3;
const Piece PIECE_ROOK = 4;
const Piece PIECE_QUEEN = 5;
const Piece PIECE_KING = 6;

// players
typedef signed int Player;


class Move {
friend class MoveApplicator;
private:
	// indeces
	signed char aStart;
	signed char aFinish;
	signed char bStart;
	signed char bFinish;
public:
	Move() { }
	Move(signed char aStart, signed char aFinish) : aStart(aStart), aFinish(aFinish),
													bStart(-1), bFinish(-1) {}
    Move(signed char aStart, signed char aFinish, signed char bStart, signed char bFinish) :
    												aStart(aStart), aFinish(aFinish),
    												bStart(bStart), bFinish(bFinish) {}
	inline Move& operator=(const Move& other) {
		memcpy(this, &other, sizeof(Move));
		return *this;
	}
};


class Board {
friend class MoveApplicator;
private:
	Piece board[BOARD_SIZE];
public:
	Board();

	class MoveIterator {
	private:
		Board* board;
		int index;

		int movesInQueue;
		Move moveQueue[BOARD_SIDE * 4]; // pleanty of extra space!
	public:
		MoveIterator(Board* board) : board(board), index(BOARD_SIZE) {
		}

		inline void addMoveToQueue(const Move& m) {
			moveQueue[movesInQueue++] = m;
		}

		void genMovesForPieceAtIndex(Player player, int index);

		inline bool getNext(Move& move, Player player) {
			while (movesInQueue == 0) {
				index--;
				if (index < 0)
					return false;

				if (board->board[index] * player > 0)
					genMovesForPieceAtIndex(player, index);
			}

			move = moveQueue[--movesInQueue];
			return true;
		}
	};

	void print();

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
	Board* board;
	Piece a;
	Piece b;
public:
	MoveApplicator(Board* b) : board(b) {}

	// apply the move!
	inline void apply(const Move& m) {
		a = board->board[m.aFinish];
		board->board[m.aFinish] = board->board[m.aStart];
		board->board[m.aStart] = 0;
		if (m.bStart >= 0) {
			b = board->board[m.bFinish];
			board->board[m.bFinish] = board->board[m.bStart];
			board->board[m.bStart] = 0;
		}
	}

	// revert the move!
	inline void revert(const Move& m) {
		board->board[m.aStart] = board->board[m.aFinish];
		board->board[m.aFinish] = a;

		if (m.bStart >= 0) {
			board->board[m.bStart] = board->board[m.bFinish];
			board->board[m.bFinish] = b;
		}
	}
};

#endif
