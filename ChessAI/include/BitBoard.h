#pragma once
#include <bitset>

constexpr size_t BOARD_DIM = 8;

class BitBoard
{
public:
	BitBoard();

	void SetBit(size_t bitNumber, bool on);
	void FlipBit(size_t bitNumber);
	bool ReadBit(size_t bitNumber) const;

	void SetBit(size_t row, unsigned char col, bool on);
	void FlipBit(size_t row, unsigned char col);
	bool ReadBit(size_t row, unsigned char col) const;

private:
	std::bitset<BOARD_DIM * BOARD_DIM> mBitBoard;
};
