#include "BoardCore.h"
#include <vector>
#include <algorithm>
#include "Texture.h"

size_t PosToIndex(size_t x, size_t y)
{
	return x + BOARD_DIM * y;
}

BitBoard MaskPawnAttacks(Colour colour, size_t square)
{
	BitBoard attacks, bitBoard;

	bitBoard.SetBit(square);
	if (colour == WHITE)
	{
		attacks |= ((bitBoard << (BOARD_DIM - 1)) & notHFile);
		attacks |= ((bitBoard << (BOARD_DIM + 1)) & notAFile);
	}
	else
	{
		attacks |= ((bitBoard >> (BOARD_DIM + 1)) & notHFile);
		attacks |= ((bitBoard >> (BOARD_DIM - 1)) & notAFile);
	}

	return attacks;
}

BitBoard MaskKnightAttacks(size_t square)
{
	BitBoard attacks, bitBoard;

	bitBoard.SetBit(square);

	// check north
	attacks |= ((bitBoard << (BOARD_DIM - 2)) & notHGFile);
	attacks |= ((bitBoard << (2 * BOARD_DIM - 1)) & notHFile);
	attacks |= ((bitBoard << (2 * BOARD_DIM + 1)) & notAFile);
	attacks |= ((bitBoard << (BOARD_DIM + 2)) & notABFile);

	// check south
	attacks |= ((bitBoard >> (BOARD_DIM + 2)) & notHGFile);
	attacks |= ((bitBoard >> (2 * BOARD_DIM + 1)) & notHFile);
	attacks |= ((bitBoard >> (2 * BOARD_DIM - 1)) & notAFile);
	attacks |= ((bitBoard >> (BOARD_DIM - 2)) & notABFile);

	return attacks;
}

BitBoard MaskKingAttacks(size_t square)
{
	BitBoard attacks, bitBoard;

	bitBoard.SetBit(square);

	// check north
	attacks |= ((bitBoard << (BOARD_DIM - 1)) & notHFile);
	attacks |= (bitBoard << BOARD_DIM);
	attacks |= ((bitBoard << (BOARD_DIM + 1)) & notAFile);

	//check left and right
	attacks |= ((bitBoard >> 1) & notHFile);
	attacks |= ((bitBoard << 1) & notAFile);

	// check south
	attacks |= ((bitBoard >> (BOARD_DIM + 1)) & notHFile);
	attacks |= (bitBoard >> BOARD_DIM);
	attacks |= ((bitBoard >> (BOARD_DIM - 1)) & notAFile);

	return attacks;
}

BitBoard MaskBishopAttacks(size_t square)
{
	BitBoard attacks;
	int rank, file, sourceRank, sourceFile;
	sourceRank = square / BOARD_DIM;
	sourceFile = square % BOARD_DIM;

	for (rank = sourceRank + 1, file = sourceFile + 1; rank < BOARD_DIM - 1 && file < BOARD_DIM - 1; ++rank, ++file)
	{
		attacks.SetBit(PosToIndex(file, rank));
	}

	for (rank = sourceRank - 1, file = sourceFile + 1; rank > 0 && file < BOARD_DIM - 1; --rank, ++file)
	{
		attacks.SetBit(PosToIndex(file, rank));
	}

	for (rank = sourceRank + 1, file = sourceFile - 1; rank < BOARD_DIM - 1 && file > 0; ++rank, --file)
	{
		attacks.SetBit(PosToIndex(file, rank));
	}

	for (rank = sourceRank - 1, file = sourceFile - 1; rank > 0 && file > 0; --rank, --file)
	{
		attacks.SetBit(PosToIndex(file, rank));
	}

	return attacks;
}

BitBoard GenerateBishopAttacks(size_t square, BitBoard blockers)
{
	BitBoard attacks;
	int rank, file, sourceRank, sourceFile;
	sourceRank = square / BOARD_DIM;
	sourceFile = square % BOARD_DIM;

	for (rank = sourceRank + 1, file = sourceFile + 1; rank < BOARD_DIM && file < BOARD_DIM; ++rank, ++file)
	{
		attacks.SetBit(PosToIndex(file, rank));
		if (blockers.ReadBit(PosToIndex(file, rank))) break;
	}

	for (rank = sourceRank - 1, file = sourceFile + 1; rank >= 0 && file < BOARD_DIM; --rank, ++file)
	{
		attacks.SetBit(PosToIndex(file, rank));
		if (blockers.ReadBit(PosToIndex(file, rank))) break;
	}

	for (rank = sourceRank + 1, file = sourceFile - 1; rank < BOARD_DIM && file >= 0; ++rank, --file)
	{
		attacks.SetBit(PosToIndex(file, rank));
		if (blockers.ReadBit(PosToIndex(file, rank))) break;
	}

	for (rank = sourceRank - 1, file = sourceFile - 1; rank >= 0 && file >= 0; --rank, --file)
	{
		attacks.SetBit(PosToIndex(file, rank));
		if (blockers.ReadBit(PosToIndex(file, rank))) break;
	}

	return attacks;
}

BitBoard MaskRookAttacks(size_t square)
{
	BitBoard attacks;
	int rank, file, sourceRank, sourceFile;
	sourceRank = square / BOARD_DIM;
	sourceFile = square % BOARD_DIM;

	for (rank = sourceRank + 1; rank < BOARD_DIM - 1; ++rank)
	{
		attacks.SetBit(PosToIndex(sourceFile, rank));
	}

	for (rank = sourceRank - 1; rank > 0; --rank)
	{
		attacks.SetBit(PosToIndex(sourceFile, rank));
	}

	for (file = sourceFile + 1; file < BOARD_DIM - 1; ++file)
	{
		attacks.SetBit(PosToIndex(file, sourceRank));
	}

	for (file = sourceFile - 1; file > 0; --file)
	{
		attacks.SetBit(PosToIndex(file, sourceRank));
	}

	return attacks;
}

BitBoard GenerateRookAttacks(size_t square, BitBoard blockers)
{
	BitBoard attacks;
	int rank, file, sourceRank, sourceFile;
	sourceRank = square / BOARD_DIM;
	sourceFile = square % BOARD_DIM;

	for (rank = sourceRank + 1; rank < BOARD_DIM; ++rank)
	{
		attacks.SetBit(PosToIndex(sourceFile, rank));
		if (blockers.ReadBit(PosToIndex(sourceFile, rank))) break;
	}

	for (rank = sourceRank - 1; rank >= 0; --rank)
	{
		attacks.SetBit(PosToIndex(sourceFile, rank));
		if (blockers.ReadBit(PosToIndex(sourceFile, rank))) break;
	}

	for (file = sourceFile + 1; file < BOARD_DIM; ++file)
	{
		attacks.SetBit(PosToIndex(file, sourceRank));
		if (blockers.ReadBit(PosToIndex(file, sourceRank))) break;
	}

	for (file = sourceFile - 1; file >= 0; --file)
	{
		attacks.SetBit(PosToIndex(file, sourceRank));
		if (blockers.ReadBit(PosToIndex(file, sourceRank))) break;
	}

	return attacks;
}

// for a given index, load the binary defined lsb from the provided bitboard (ex: index = 11010 means lod the 2nd, 4th, and 5th lsbs in the board)
// we pass in number of bits in attack mask instead of querying it for performance
BitBoard SetOccupancy(size_t index, size_t relevantBits, BitBoard attackMask)
{
	BitBoard result;
	for (size_t i = 0; i < relevantBits; ++i)
	{
		size_t square = attackMask.GetLSBIndex();
		attackMask.ClearBit(square);
		if (index & (1ULL << i)) result.SetBit(square);
	}

	return result;
}

// this function is used to find bishop and rook magics. It can be used to compute at runtime, or for precomputed magics.
// The performance trade off is minimal, since the magics are computed only at the creation of the Board object, but precomputed values are provided if preferred
BitBoard FindMagicNumber(size_t square, size_t relevantBits, Piece bishopOrRook)
{
    FATAL_ASSERT(bishopOrRook == BISHOP || bishopOrRook == ROOK);

    // max number of possible relevant bits is for rook at 12 -> 2^12 - 1 ~ 4096 possible configurations
	int occupancyIndex = 1 << relevantBits; // max value of 4096
	std::vector<BitBoard> occupancy; occupancy.resize(occupancyIndex);
	std::vector<BitBoard> attacks; attacks.resize(occupancyIndex);
	std::vector<BitBoard> usedAttacks; usedAttacks.resize(occupancyIndex);

	BitBoard attackMask = bishopOrRook == BISHOP ? MaskBishopAttacks(square) : MaskRookAttacks(square);
	for (int i = 0; i < occupancyIndex; ++i) // gets every possible configuration of blockers for an attack from a specific square
	{
		occupancy[i] = SetOccupancy(i, relevantBits, attackMask);
		attacks[i] = bishopOrRook == BISHOP ? GenerateBishopAttacks(square, occupancy[i]) : GenerateRookAttacks(square, occupancy[i]);
	}

	for (int i = 0; i < 1000000000; ++i)
	{
		BitBoard magicNum = BitBoard(GetMagicNumber());
		BitBoard candidate = (attackMask * magicNum) & 0xFF00000000000000ULL;
		if (candidate.CountBits() < 6) continue;
		
		bool fail = false;
		for (int index = 0; index < occupancyIndex && !fail; ++index)
		{
			int magicIndex = (int)((occupancy[index] * magicNum) >> (64 - relevantBits)).to_ullong();
			if (!usedAttacks[magicIndex]) usedAttacks[magicIndex] = attacks[index];
			else if (usedAttacks[magicIndex].to_ullong() != attacks[index].to_ullong()) fail = true;
		}

		if (!fail) return magicNum;
		else std::fill(usedAttacks.begin(), usedAttacks.end(), BitBoard(0));
	}

	FATAL_ASSERT(false && "magic number could not be found!");
	return BitBoard(0);
}

int GetMagicIndex(BitBoard occupancy, uint64_t magicNumber, int relevantBits)
{
	return (int)((occupancy.to_ullong() * magicNumber) >> (64 - relevantBits));
}

void InitSprites(SDL_Renderer* renderer)
{
	std::vector<std::string> colours = { "white_", "black_" };
	std::vector<std::string> pieces = { "pawn", "knight", "bishop", "rook", "queen", "king" };
	for (size_t colour = WHITE; colour <= BLACK; colour += BLACK)
	{
		for (size_t piece = PAWN; piece <= KING; ++piece)
		{
			std::string path = "assets/pieces/" + colours[colour / BLACK] + "pieces/" + colours[colour / BLACK] + pieces[piece] + ".png";
			pieceSprites[colour | piece] = new Texture(path, renderer);
			pieceSprites[colour | piece]->SetWidth(texWidth);
			pieceSprites[colour | piece]->SetHeight(texHeight);
		}
	}
}

void DestroySprites()
{
	for (size_t colour = WHITE; colour <= BLACK; colour += BLACK)
	{
		for (size_t piece = PAWN; piece <= KING; ++piece)
		{
			delete pieceSprites[colour | piece];
		}
	}
}

void ResizeSprites(int boardWidth, int boardHeight)
{
	texWidth = boardWidth / BOARD_DIM; texHeight = boardHeight / BOARD_DIM;
	std::for_each(pieceSprites.begin(), pieceSprites.end(), [](Texture* sprite)
		{
			sprite->SetWidth(texWidth);
			sprite->SetHeight(texHeight);
		});
}
