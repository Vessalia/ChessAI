#include "BitBoard.h"

BitBoard::BitBoard()
{
	mBitBoard.reset();
}

void BitBoard::SetBit(size_t bitNumber, bool on)
{
	mBitBoard |= static_cast<uint64_t>(on ? 1 : 0) << bitNumber;
}

void BitBoard::SetBit(size_t row, unsigned char col, bool on)
{
	size_t colVal = static_cast<size_t>(col - 'a');
	SetBit(colVal + BOARD_DIM * (row - 1), on);
}

void BitBoard::FlipBit(size_t bitNumber)
{
	mBitBoard ^= static_cast<uint64_t>(1) << bitNumber;
}

void BitBoard::FlipBit(size_t row, unsigned char col)
{
	size_t colVal = static_cast<size_t>(col - 'a');
	FlipBit(colVal + BOARD_DIM * (row - 1));
}

bool BitBoard::ReadBit(size_t bitNumber) const
{
	return mBitBoard.to_ullong() & static_cast<uint64_t>(1) << bitNumber;
}

bool BitBoard::ReadBit(size_t row, unsigned char col) const
{
	size_t colVal = static_cast<size_t>(col - 'a');
	return ReadBit(colVal + BOARD_DIM * (row - 1));
}
