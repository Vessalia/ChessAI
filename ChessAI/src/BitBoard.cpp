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

BitBoard& BitBoard::SetBit(size_t col, size_t row)
{
	return SetBit(col + BOARD_DIM * row);
}

BitBoard& BitBoard::FlipBit(size_t bitNumber)
{
	mBitBoard ^= static_cast<uint64_t>(1) << bitNumber;
	return *this;
}

BitBoard& BitBoard::FlipBit(size_t col, size_t row)
{
	return FlipBit(col + BOARD_DIM * row);
}

BitBoard& BitBoard::ClearBit(size_t bitNumber)
{
	mBitBoard &= ~(static_cast<uint64_t>(1) << bitNumber);
	return *this;
}

BitBoard& BitBoard::ClearBit(size_t col, size_t row)
{
	return ClearBit(col + BOARD_DIM * row);
}

bool BitBoard::ReadBit(size_t bitNumber) const
{
	return mBitBoard.to_ullong() & static_cast<uint64_t>(1) << bitNumber;
}

bool BitBoard::ReadBit(size_t col, size_t row) const
{
	return ReadBit(col + BOARD_DIM * row);
}
