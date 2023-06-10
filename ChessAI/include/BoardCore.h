#pragma once
#include "BitBoard.h"

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

constexpr std::array<size_t, BOARD_DIM* BOARD_DIM> rookBitCount =
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
