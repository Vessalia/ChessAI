#pragma once
#include <bitset>

constexpr int BOARD_DIM = 8;

class BitBoard
{
public:
	BitBoard();
	explicit constexpr BitBoard(unsigned long long val) : mBitBoard(static_cast<uint64_t>(val)) { }

	BitBoard& operator=(uint64_t val);

	BitBoard& operator&=(const BitBoard& other);
	BitBoard& operator|=(const BitBoard& other);
	BitBoard& operator<<=(size_t numBits);
	BitBoard& operator>>=(size_t numBits);

	BitBoard operator<<(size_t numBits);
	BitBoard operator>>(size_t numBits);
	BitBoard operator&(const BitBoard& other);
	BitBoard operator|(const BitBoard& other);

	BitBoard& SetBit(size_t bitNumber);
	BitBoard& FlipBit(size_t bitNumber);
	BitBoard& ClearBit(size_t bitNumber);
	bool ReadBit(size_t bitNumber) const;

	BitBoard& SetBit(size_t col, size_t row);
	BitBoard& FlipBit(size_t col, size_t row);
	BitBoard& ClearBit(size_t col, size_t row);
	bool ReadBit(size_t col, size_t row) const;

	bool NonZero() const;

	void Print() const;

private:
	std::bitset<BOARD_DIM * BOARD_DIM> mBitBoard;
};
