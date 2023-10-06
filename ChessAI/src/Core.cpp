#include "Core.h"

static unsigned int _seed = 1804289383;

size_t SCREEN_WIDTH = 640;
size_t SCREEN_HEIGHT = 720;

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
	uint64_t u1, u2, u3, u4; // slice the top 16 bits
	u1 = GetRandU32() & 0xFFFF; u2 = GetRandU32() & 0xFFFF;
	u3 = GetRandU32() & 0xFFFF; u4 = GetRandU32() & 0xFFFF;
	return u1 | (u2 << 16) | (u3 << 32) | (u4 << 48);
}

uint64_t GetMagicNumber()
{
	return GetRandU64() & GetRandU64() & GetRandU64();
}

std::vector<std::string> Tokenize(const std::string& tokens, const std::string& delimiter)
{
    std::vector<std::string> tokenized;
    size_t start = 0;
    size_t end = tokens.find(delimiter);

    while (end != std::string::npos)
    {
        std::string token = tokens.substr(start, end - start);
        if (!token.empty()) tokenized.push_back(token);

        start = end + delimiter.size();
        end = tokens.find(delimiter, start);
    }

    std::string lastToken = tokens.substr(start);
    if (!lastToken.empty()) tokenized.push_back(lastToken);

    return tokenized;
}
