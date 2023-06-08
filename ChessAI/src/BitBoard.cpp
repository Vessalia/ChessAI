#include "BitBoard.h"
#include "iostream"

BitBoard::BitBoard()
{
	mBitBoard.reset();
}

BitBoard& BitBoard::operator=(uint64_t val)
{
	mBitBoard = val;
	return *this;
}

BitBoard& BitBoard::operator&=(const BitBoard& other)
{
	mBitBoard &= other.mBitBoard;
	return *this;
}

BitBoard& BitBoard::operator|=(const BitBoard& other)
{
	mBitBoard |= other.mBitBoard;
	return *this;
}

BitBoard& BitBoard::operator<<=(size_t numBits)
{
	mBitBoard <<= numBits;
	return *this;
}

BitBoard& BitBoard::operator>>=(size_t numBits)
{
	mBitBoard >>= numBits;
	return *this;
}

BitBoard BitBoard::operator<<(size_t numBits)
{
	BitBoard result;
	result.mBitBoard = mBitBoard << numBits;
	return result;
}

BitBoard BitBoard::operator>>(size_t numBits)
{
	BitBoard result;
	result.mBitBoard = mBitBoard >> numBits;
	return result;
}

BitBoard BitBoard::operator&(const BitBoard& other)
{
	BitBoard result;
	result.mBitBoard = mBitBoard & other.mBitBoard;
	return result;
}

BitBoard BitBoard::operator|(const BitBoard& other)
{
	BitBoard result;
	result.mBitBoard = mBitBoard | other.mBitBoard;
	return result;
}

BitBoard& BitBoard::SetBit(size_t bitNumber)
{
	mBitBoard |= (static_cast<uint64_t>(1) << bitNumber);
	return *this;
}

BitBoard& BitBoard::SetBit(size_t col, size_t row)
{
	return SetBit(col + BOARD_DIM * row);
}

BitBoard& BitBoard::FlipBit(size_t bitNumber)
{
	mBitBoard ^= (static_cast<uint64_t>(1) << bitNumber);
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
	return mBitBoard.to_ullong() & (static_cast<uint64_t>(1) << bitNumber);
}

bool BitBoard::ReadBit(size_t col, size_t row) const
{
	return ReadBit(col + BOARD_DIM * row);
}

bool BitBoard::NonZero() const
{
	return mBitBoard == 0 ? false : true;
}

void BitBoard::Print() const
{
	for (int rank = BOARD_DIM - 1; rank >= 0; --rank)
	{
		for (int file = 0; file < BOARD_DIM; ++file)
		{
			int square = rank * BOARD_DIM + file;
			int val = ReadBit(square);
			std::cout << " " << val << " ";
		}
		std::cout << std::endl;
	}
	std::cout << "Value: " << mBitBoard.to_ullong() << std::endl;
}
