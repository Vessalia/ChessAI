#pragma once
#include <bitset>

constexpr size_t BOARD_DIM = 8;

class BitBoard
{
public:
	BitBoard();

	BitBoard& SetBit(size_t bitNumber);
	BitBoard& FlipBit(size_t bitNumber);
	BitBoard& ClearBit(size_t bitNumber);
	bool ReadBit(size_t bitNumber) const;

	BitBoard& SetBit(unsigned char col, size_t row);
	BitBoard& FlipBit(unsigned char col, size_t row);
	BitBoard& ClearBit(unsigned char col, size_t row);
	bool ReadBit(unsigned char col, size_t row) const;

private:
	std::bitset<BOARD_DIM * BOARD_DIM> mBitBoard;
};
