#include "Core.h"

static unsigned int _seed = 1804289383;

void Assert(bool cond)
{
	SDL_assert(cond);
}

unsigned int GetRandU32()
{
	_seed ^= _seed << 13;
	_seed ^= _seed >> 17;
	_seed ^= _seed << 5;

	return _seed;
}

uint64_t GetRandU64()
{
	uint64_t u1, u2, u3, u4;
	u1 = GetRandU32(); u2 = GetRandU32();
	u3 = GetRandU32(); u4 = GetRandU32();
	return u1 | (u2 << 16) | (u3 << 32) | (u4 << 48);
}

uint64_t GetMagicNumber()
{
	return GetRandU64() & GetRandU64() & GetRandU64();
}
