#include "BitBoard.h"
#include "iostream"

BitBoard::BitBoard()
{
	mBitBoard.reset();
}

BitBoard::operator bool() const
{
	return mBitBoard != 0;
}

BitBoard BitBoard::operator-=(uint64_t val)
{
	*this = BitBoard(mBitBoard.to_ullong() - val);
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

BitBoard BitBoard::operator-(uint64_t val) const
{
	return BitBoard(mBitBoard.to_ullong() - val);
}

BitBoard BitBoard::operator<<(size_t numBits) const
{
	BitBoard result;
	result.mBitBoard = mBitBoard << numBits;
	return result;
}

BitBoard BitBoard::operator>>(size_t numBits) const
{
	BitBoard result;
	result.mBitBoard = mBitBoard >> numBits;
	return result;
}

BitBoard BitBoard::operator&(const BitBoard& other) const
{
	BitBoard result;
	result.mBitBoard = mBitBoard & other.mBitBoard;
	return result;
}

BitBoard BitBoard::operator|(const BitBoard& other) const
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

BitBoard& BitBoard::FlipBit(size_t bitNumber)
{
	mBitBoard ^= (static_cast<uint64_t>(1) << bitNumber);
	return *this;
}

BitBoard& BitBoard::ClearBit(size_t bitNumber)
{
	mBitBoard &= ~(static_cast<uint64_t>(1) << bitNumber);
	return *this;
}

bool BitBoard::PopBit(size_t bitNumber)
{
	bool result = ReadBit(bitNumber);
	if(result) FlipBit(bitNumber);
	return result;
}

bool BitBoard::ReadBit(size_t bitNumber) const
{
	return mBitBoard.to_ullong() & (static_cast<uint64_t>(1) << bitNumber);
}

size_t BitBoard::CountBits() const
{
	BitBoard temp = BitBoard(mBitBoard.to_ullong());
	size_t count = 0;

	while (temp)
	{
		temp &= (temp - 1);
		++count;
	}

	return count;
}

#ifdef _MSC_VER
#include <intrin.h>
size_t BitBoard::GetLSBIndex() const
{
	unsigned long index;
	uint64_t board = mBitBoard.to_ullong();

	if (_BitScanForward64(&index, board))
	{
		return static_cast<size_t>(index);
	}

	return BOARD_DIM * BOARD_DIM;
}
#else
size_t BitBoard::GetLSBIndex() const
{
	size_t index = 0;
	uint64_t mask = 1;
	uint64_t board = mBitBoard.to_ullong();
	while (!(board & mask) && index < BOARD_DIM * BOARD_DIM)
	{
		mask <<= 1;
		++index;
	}

	return index;
}
#endif

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
