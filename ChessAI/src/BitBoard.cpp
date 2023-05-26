#include "BitBoard.h"

BitBoard::BitBoard()
{
	mBitBoard.reset();
}

BitBoard& BitBoard::SetBit(size_t bitNumber)
{
	mBitBoard |= static_cast<uint64_t>(1) << bitNumber;
	return *this;
}

BitBoard& BitBoard::SetBit(unsigned char col, size_t row)
{
	size_t colVal = static_cast<size_t>(col - 'a');
	return SetBit(colVal + BOARD_DIM * (row - 1));
}

BitBoard& BitBoard::FlipBit(size_t bitNumber)
{
	mBitBoard ^= static_cast<uint64_t>(1) << bitNumber;
	return *this;
}

BitBoard& BitBoard::FlipBit(unsigned char col, size_t row)
{
	size_t colVal = static_cast<size_t>(col - 'a');
	return FlipBit(colVal + BOARD_DIM * (row - 1));
}

BitBoard& BitBoard::ClearBit(size_t bitNumber)
{
	mBitBoard &= ~(static_cast<uint64_t>(1) << bitNumber);
	return *this;
}

BitBoard& BitBoard::ClearBit(unsigned char col, size_t row)
{
	size_t colVal = static_cast<size_t>(col - 'a');
	return ClearBit(colVal + BOARD_DIM * (row - 1));
}

bool BitBoard::ReadBit(size_t bitNumber) const
{
	return mBitBoard.to_ullong() & static_cast<uint64_t>(1) << bitNumber;
}

bool BitBoard::ReadBit(unsigned char col, size_t row) const
{
	size_t colVal = static_cast<size_t>(col - 'a');
	return ReadBit(colVal + BOARD_DIM * (row - 1));
}
