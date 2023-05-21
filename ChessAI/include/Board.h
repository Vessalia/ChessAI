#pragma once
#include <array>
#include "BitBoard.h"

#define NUM_BITBOARDS 16

enum Pieces
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
	std::array<BitBoard, NUM_BITBOARDS> mBitBoards;
};
