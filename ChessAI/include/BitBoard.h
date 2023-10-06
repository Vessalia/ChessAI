#pragma once
#include <bitset>

constexpr size_t BOARD_DIM = 8;

class BitBoard
{
public:
	BitBoard();
	explicit constexpr BitBoard(unsigned long long val) : mBitBoard(static_cast<uint64_t>(val)) { }

	explicit operator bool() const;

	BitBoard& operator&=(const BitBoard& other);
	BitBoard& operator|=(const BitBoard& other);
	BitBoard& operator<<=(size_t numBits);
	BitBoard& operator>>=(size_t numBits);
	BitBoard& operator*=(const BitBoard& other);
	BitBoard& operator*=(size_t other);

	BitBoard operator~() const;
	BitBoard operator<<(size_t numBits) const;
	BitBoard operator>>(size_t numBits) const;
	BitBoard operator&(const BitBoard& other) const;
	BitBoard operator&(size_t other) const;
	BitBoard operator|(const BitBoard& other) const;
	BitBoard operator*(const BitBoard& other) const;
	BitBoard operator*(size_t other) const;

	BitBoard& SetBit(size_t bitNumber);
	BitBoard& FlipBit(size_t bitNumber);
	BitBoard& ClearBit(size_t bitNumber);
	bool PopBit(size_t bitNumber);
	bool ReadBit(size_t bitNumber) const;

	size_t CountBits() const;
	size_t GetLSBIndex() const;

	void Print() const;

	uint64_t to_ullong() const;

private:
	std::bitset<BOARD_DIM * BOARD_DIM> mBitBoard;
};
