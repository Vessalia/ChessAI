#pragma once
#include "BitBoard.h"
#include <array>

enum Piece
{
	PAWN,
	KNIGHT,
	BISHOP,
	ROOK,
	QUEEN,
	KING,
	NUM_PIECES
};

enum Colour
{
	WHITE = 0,
	BLACK = 8,
	NUM_COLOURS = 2
};

size_t PosToIndex(size_t x, size_t y);

BitBoard MaskPawnAttacks(Colour colour, size_t square);
BitBoard MaskKnightAttacks(size_t square);
BitBoard MaskKingAttacks(size_t square);
BitBoard MaskBishopAttacks(size_t square);
BitBoard MaskRookAttacks(size_t square);

BitBoard GenerateBishopAttacks(size_t square, BitBoard blockers);
BitBoard GenerateRookAttacks(size_t square, BitBoard blockers);

BitBoard SetOccupancy(size_t index, size_t relevantBits, BitBoard attackMask);

BitBoard FindMagicNumber(size_t square, size_t relevantBits, Piece bishopOrRook);




constexpr static size_t NUM_BITBOARDS = 16;

// precomputed boards
constexpr BitBoard notAFile = BitBoard(18374403900871474942ULL);
constexpr BitBoard notHFile = BitBoard(9187201950435737471ULL);

constexpr BitBoard notABFile = BitBoard(18229723555195321596ULL);
constexpr BitBoard notHGFile = BitBoard(4557430888798830399ULL);

constexpr std::array<size_t, BOARD_DIM * BOARD_DIM> bishopBitCount = 
{
	6, 5, 5, 5, 5, 5, 5, 6,
	5, 5, 5, 5, 5, 5, 5, 5,
	5, 5, 7, 7, 7, 7, 5, 5,
	5, 5, 7, 9, 9, 7, 5, 5,
	5, 5, 7, 9, 9, 7, 5, 5,
	5, 5, 7, 7, 7, 7, 5, 5,
	5, 5, 5, 5, 5, 5, 5, 5, 
	6, 5, 5, 5, 5, 5, 5, 6
};

constexpr std::array<size_t, BOARD_DIM * BOARD_DIM> rookBitCount =
{
	12, 11, 11, 11, 11, 11, 11, 12,
	11, 10, 10, 10, 10, 10, 10, 11,
	11, 10, 10, 10, 10, 10, 10, 11,
	11, 10, 10, 10, 10, 10, 10, 11,
	11, 10, 10, 10, 10, 10, 10, 11,
	11, 10, 10, 10, 10, 10, 10, 11,
	11, 10, 10, 10, 10, 10, 10, 11,
	12, 11, 11, 11, 11, 11, 11, 12
};
