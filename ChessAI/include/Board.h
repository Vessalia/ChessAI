#pragma once
#include <array>
#include <vector>
#include "BitBoard.h"

#define NUM_BITBOARDS 16

enum Piece
{
	PAWN,
	KNIGHT,
	BISHOP,
	ROOK,
	QUEEN,
	KING
};

enum Colour
{
	WHITE = 0,
	BLACK = 8
};

class Board
{
public:
	Board();

private:
	struct Move
	{
		size_t bitboardIndex;
		size_t from;
		size_t to;
	};

	static const uint8_t PIECE_MASK = 00000111;
	static const uint8_t COLOUR_MASK = 00001000;

	void DoMove(size_t pieceColour, size_t from, size_t to);

	std::vector<size_t> GetValidLocations(size_t from) const;
	bool CheckValidLocation(Colour colour, size_t to) const;
	int GetPieceAt(size_t loc) const;

	std::vector<size_t> GetPawnMoves(Colour colour, size_t from) const;

	std::array<BitBoard, NUM_BITBOARDS> mBitBoards;
};
