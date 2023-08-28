#include "Board.h"
#include <algorithm>

static std::array<Texture*, NUM_BITBOARDS> pieceSprites;

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
			pieceSprites[colour | piece]->SetWidth(TEX_WIDTH);
			pieceSprites[colour | piece]->SetHeight(TEX_HEIGHT);
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

void ResizeSprites()
{
	std::for_each(pieceSprites.begin(), pieceSprites.end(), [](Texture* sprite)
		{
			if (sprite)
			{
				sprite->SetWidth(TEX_WIDTH);
				sprite->SetHeight(TEX_HEIGHT);
			}
		});
}

Board::Board()
{
	for (size_t row = 0; row < BOARD_DIM; ++row)
	{
		for (size_t col = 0; col < BOARD_DIM; ++col)
		{
			unsigned int index = WHITE;
			switch (row)
			{
				case 1: index |= PAWN; break;
				case BOARD_DIM - 2: index = BLACK | PAWN; break;

				case BOARD_DIM - 1 : index = BLACK;
				case 0 :
				{
					switch (col)
					{
						case 0:
						case BOARD_DIM - 1: index |= ROOK; break;

						case 1:
						case BOARD_DIM - 2: index |= KNIGHT; break;

						case 2:
						case BOARD_DIM - 3: index |= BISHOP; break;

						case 3: index |= QUEEN; break;
						case BOARD_DIM - 4: index |= KING; break;
					}
				}
			}

			size_t square = PosToIndex(col, row);

			if (row < 2 || row >= BOARD_DIM - 2) mPieceBoards[index].SetBit(PosToIndex(col, row));

			mPawnAttacks[0][square] = MaskPawnAttacks(WHITE, square);
			mPawnAttacks[1][square] = MaskPawnAttacks(BLACK, square);

			mKnightAttacks[square] = MaskKnightAttacks(square);

			mKingAttacks[square] = MaskKingAttacks(square);

			mBishopMasks[square] = MaskBishopAttacks(square);
			mRookMasks[square] = MaskRookAttacks(square);

			mBishopAttacks[square].resize(1ULL << bishopBitCount[square]);
			mRookAttacks[square].resize(1ULL << rookBitCount[square]);

			for (size_t occ = 0; occ < (1ULL << bishopBitCount[square]); ++occ)
			{
				BitBoard bishopOccupancy = SetOccupancy(occ, bishopBitCount[square], mBishopMasks[square]);
				int bishopMagicIndex = GetMagicIndex(bishopOccupancy, bishopMagics[square], bishopBitCount[square]);
				mBishopAttacks[square][bishopMagicIndex] = GenerateBishopAttacks(square, bishopOccupancy);
			}

			for (size_t occ = 0; occ < (1ULL << rookBitCount[square]); ++occ)
			{
				BitBoard rookOccupancy = SetOccupancy(occ, rookBitCount[square], mRookMasks[square]);
				int rookMagicIndex = GetMagicIndex(rookOccupancy, rookMagics[square], rookBitCount[square]);
				mRookAttacks[square][rookMagicIndex] = GenerateRookAttacks(square, rookOccupancy);
			}
		}
	}
}

Board::Board(const std::string& fen)
{
	*this = ParseFen(fen);
}

std::vector<size_t> currValidLocations;
void Board::Draw(SDL_Renderer* renderer, size_t selectedIndex)
{
	for (int piece = PAWN; piece <= KING; ++piece)
	{
		for (int colour = WHITE; colour <= BLACK; colour += BLACK)
		{
			FATAL_ASSERT(pieceSprites[colour | piece] && "Piece textures not loaded!");
		}
	}
	//if (!currValidLocations.size() && selectedIndex != INVALID_INDEX) currValidLocations = GetValidLocations(selectedIndex);

	for (int x = 0; x < BOARD_DIM; ++x)
	{
		for (int y = 0; y < BOARD_DIM; ++y)
		{
			uint8_t colourR = 0;
			uint8_t colourG = 0;
			uint8_t colourB = 0;
			uint8_t colourA = 255;
			if ((x + y) % 2)
			{
				colourR = colourG = colourB = 255;
			}
			if (PosToIndex(x, y) == selectedIndex)
			{
				colourR = 255; colourG = colourB = 0;
			}
			if (std::find(currValidLocations.begin(), currValidLocations.end(), PosToIndex(x, y)) != currValidLocations.end())
			{
				colourR = 0; colourG = colourB = 128;
			}

			SDL_Rect square = { x * TEX_WIDTH, TEX_HEIGHT * (BOARD_DIM - 1) - y * TEX_HEIGHT, TEX_WIDTH, TEX_HEIGHT };

			SDL_SetRenderDrawColor(renderer, colourR, colourG, colourB, colourA);
			SDL_RenderFillRect(renderer, &square);

			int pieceColour = GetPieceColourAt(PosToIndex(x, y));
			if (pieceColour == -1) continue;

			pieceSprites[pieceColour]->RenderTexture(renderer, x * TEX_WIDTH, TEX_HEIGHT * (BOARD_DIM - 1) - y * TEX_HEIGHT);
		}
	}
}

void Board::Print() const
{
	/*BitBoard occupancy = BitBoard(0);
	occupancy.SetBit(PosToIndex(1, 5));
	occupancy.SetBit(PosToIndex(6, 6));
	occupancy.SetBit(PosToIndex(4, 2));
	occupancy.SetBit(PosToIndex(2, 2));
	occupancy.SetBit(PosToIndex(1, 1));
	occupancy.SetBit(PosToIndex(6, 3));

	GetQueenAttacks(PosToIndex(3, 3), occupancy).Print();*/
}

Board Board::ParseFen(const std::string& fen)
{
	Board parsedBoard;

	std::for_each(fen.begin(), fen.end(), [&parsedBoard, boardIndex = 0UL](char curr) mutable
		{
			if (fenHelper.find(curr) != fenHelper.end())
			{
				int pieceColour = fenHelper[curr];
				parsedBoard.mPieceBoards[pieceColour].SetBit(boardIndex++);
			}
			else if (curr >= '1' && curr <= BOARD_DIM + '0')
			{
				boardIndex += curr - '0';
			}
		});

	return parsedBoard;
}

BitBoard Board::GetBishopAttacks(size_t square, BitBoard occupancy) const
{
	int magicIndex = GetMagicIndex(occupancy & mBishopMasks[square], bishopMagics[square], bishopBitCount[square]);
	return mBishopAttacks[square][magicIndex];
}

BitBoard Board::GetRookAttacks(size_t square, BitBoard occupancy) const
{
	int magicIndex = GetMagicIndex(occupancy & mRookMasks[square], rookMagics[square], rookBitCount[square]);
	return mRookAttacks[square][magicIndex];
}

BitBoard Board::GetQueenAttacks(size_t square, BitBoard occupancy) const
{
	int magicBishopIndex = GetMagicIndex(occupancy & mBishopMasks[square], bishopMagics[square], bishopBitCount[square]);
	int magicRookIndex = GetMagicIndex(occupancy & mRookMasks[square], rookMagics[square], rookBitCount[square]);
	return mBishopAttacks[square][magicBishopIndex] | mRookAttacks[square][magicRookIndex];
}

bool Board::InCheck(Colour colour) const
{
	return false;
}

void Board::DoMove(size_t pieceColour, size_t from, size_t to)
{
	mPieceBoards[pieceColour].ClearBit(from).SetBit(to);
}

int Board::GetPieceColourAt(size_t loc) const
{
	int pieceColour = -1;
	for (size_t piece = PAWN; piece <= KING && pieceColour < 0; ++piece)
	{
		for (size_t colour = WHITE; colour <= BLACK && pieceColour < 0; colour += BLACK)
		{
			if (mPieceBoards[colour | piece].ReadBit(loc))
			{
				pieceColour = colour | piece;
				break;
			}
		}
	}
	
	return pieceColour;
}

BitBoard Board::GetColourBoard(Colour colour) const
{
	BitBoard result = mPieceBoards[colour | PAWN];
	for (size_t piece = KNIGHT; piece <= KING; ++piece)
	{
		result |= mPieceBoards[colour | piece];
	}

	return result;
}

BitBoard Board::GetOccupancyBoard() const
{
	return GetColourBoard(WHITE) | GetColourBoard(BLACK);
}
