#include "Board.h"

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

			if (row < 2 || row >= BOARD_DIM - 2) mBitBoards[index].SetBit(PosToIndex(col, row));

			mPawnAttacks[0][PosToIndex(col, row)] = MaskPawnAttacks(WHITE, PosToIndex(col, row));
			mPawnAttacks[1][PosToIndex(col, row)] = MaskPawnAttacks(BLACK, PosToIndex(col, row));

			mKnightAttacks[PosToIndex(col, row)] = MaskKnightAttacks(PosToIndex(col, row));

			mKingAttacks[PosToIndex(col, row)] = MaskKingAttacks(PosToIndex(col, row));
		}
	}
}

Board::~Board()
{
	for (size_t colour = WHITE; colour <= BLACK; colour += BLACK)
	{
		for (size_t piece = PAWN; piece <= KING; ++piece)
		{
			delete mPieceSprites[colour | piece];
		}
	}
}

void Board::InitSprites(SDL_Renderer* renderer)
{
	std::vector<std::string> colours = { "white_", "black_" };
	std::vector<std::string> pieces = { "pawn", "knight", "bishop", "rook", "queen", "king" };
	for (size_t colour = WHITE; colour <= BLACK; colour += BLACK)
	{
		for (size_t piece = PAWN; piece <= KING; ++piece)
		{
			std::string path = "assets/pieces/" + colours[colour / BLACK] + "pieces/" + colours[colour / BLACK] + pieces[piece] + ".png";
			mPieceSprites[colour | piece] = new Texture(path, renderer);
			mPieceSprites[colour | piece]->SetWidth(mWidth);
			mPieceSprites[colour | piece]->SetHeight(mHeight);
		}
	}
}

std::vector<size_t> currValidLocations;
void Board::Draw(SDL_Renderer* renderer, size_t selectedIndex)
{
	if (!currValidLocations.size() && selectedIndex != INVALID_INDEX) currValidLocations = GetValidLocations(selectedIndex);

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

			SDL_Rect square = { x * mWidth, mHeight * (BOARD_DIM - 1) - y * mHeight, mWidth, mHeight };

			SDL_SetRenderDrawColor(renderer, colourR, colourG, colourB, colourA);
			SDL_RenderFillRect(renderer, &square);

			int pieceColour = GetPieceColourAt(PosToIndex(x, y));
			if (pieceColour == -1) continue;

			mPieceSprites[pieceColour]->RenderTexture(renderer, x * mWidth, mHeight * (BOARD_DIM - 1) - y * mHeight);
		}
	}
}

void Board::Resize(int width, int height)
{
	mWidth = width / BOARD_DIM; mHeight = height / BOARD_DIM;
	for (auto& sprite : mPieceSprites)
	{
		if (!sprite) continue;
		sprite->SetWidth(mWidth);
		sprite->SetHeight(mHeight);
	}
}

void Board::Print() const
{
	for (int j = 0; j < BOARD_DIM * BOARD_DIM; ++j)
	{
		mKingAttacks[j].Print();
		printf("\n");
	}
}

BitBoard Board::MaskPawnAttacks(Colour colour, size_t square) const
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

BitBoard Board::MaskKnightAttacks(size_t square) const
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

BitBoard Board::MaskKingAttacks(size_t square) const
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

BitBoard Board::MaskBishopAttacks(size_t square) const
{
	BitBoard attacks;
	int rank, file, sourceRank, sourceFile;
	sourceRank = square / BOARD_DIM;
	sourceFile = square % BOARD_DIM;
	
	for (rank = sourceRank + 1, file = sourceFile + 1; rank <= BOARD_DIM - 2 && file <= BOARD_DIM - 2; ++rank, ++file)
	{
		attacks.SetBit(PosToIndex(file, rank));
	}

	for (rank = sourceRank - 1, file = sourceFile + 1; rank > 0 && file <= BOARD_DIM - 2; --rank, ++file)
	{
		attacks.SetBit(PosToIndex(file, rank));
	}

	for (rank = sourceRank + 1, file = sourceFile - 1; rank <= BOARD_DIM - 2 && file > 0; ++rank, --file)
	{
		attacks.SetBit(PosToIndex(file, rank));
	}

	for (rank = sourceRank - 1, file = sourceFile - 1; rank > 0 && file > 0; --rank, --file)
	{
		attacks.SetBit(PosToIndex(file, rank));
	}

	return attacks;
}

BitBoard Board::GenerateBishopAttacks(size_t square, BitBoard blockers) const
{
	BitBoard attacks;
	int rank, file, sourceRank, sourceFile;
	sourceRank = square / BOARD_DIM;
	sourceFile = square % BOARD_DIM;

	for (rank = sourceRank + 1, file = sourceFile + 1; rank <= BOARD_DIM - 2 && file <= BOARD_DIM - 2; ++rank, ++file)
	{
		attacks.SetBit(PosToIndex(file, rank));
		if (blockers.ReadBit(PosToIndex(file, rank))) break;
	}

	for (rank = sourceRank - 1, file = sourceFile + 1; rank > 0 && file <= BOARD_DIM - 2; --rank, ++file)
	{
		attacks.SetBit(PosToIndex(file, rank));
		if (blockers.ReadBit(PosToIndex(file, rank))) break;
	}

	for (rank = sourceRank + 1, file = sourceFile - 1; rank <= BOARD_DIM - 2 && file > 0; ++rank, --file)
	{
		attacks.SetBit(PosToIndex(file, rank));
		if (blockers.ReadBit(PosToIndex(file, rank))) break;
	}

	for (rank = sourceRank - 1, file = sourceFile - 1; rank > 0 && file > 0; --rank, --file)
	{
		attacks.SetBit(PosToIndex(file, rank));
		if (blockers.ReadBit(PosToIndex(file, rank))) break;
	}

	return attacks;
}

BitBoard Board::MaskRookAttacks(size_t square) const
{
	BitBoard attacks;
	int rank, file, sourceRank, sourceFile;
	sourceRank = square / BOARD_DIM;
	sourceFile = square % BOARD_DIM;

	for (rank = sourceRank + 1; rank <= BOARD_DIM - 2; ++rank)
	{
		attacks.SetBit(PosToIndex(sourceFile, rank));
	}

	for (rank = sourceRank - 1; rank > 0; --rank)
	{
		attacks.SetBit(PosToIndex(sourceFile, rank));
	}

	for (file = sourceFile + 1; file <= BOARD_DIM - 2; ++file)
	{
		attacks.SetBit(PosToIndex(file, sourceRank));
	}

	for (file = sourceFile - 1; file > 0; --file)
	{
		attacks.SetBit(PosToIndex(file, sourceRank));
	}

	return attacks;
}

BitBoard Board::GenerateRookAttacks(size_t square, BitBoard blockers) const
{
	BitBoard attacks;
	int rank, file, sourceRank, sourceFile;
	sourceRank = square / BOARD_DIM;
	sourceFile = square % BOARD_DIM;

	for (rank = sourceRank + 1; rank <= BOARD_DIM - 2; ++rank)
	{
		attacks.SetBit(PosToIndex(sourceFile, rank));
		if (blockers.ReadBit(PosToIndex(sourceFile, rank))) break;
	}

	for (rank = sourceRank - 1; rank > 0; --rank)
	{
		attacks.SetBit(PosToIndex(sourceFile, rank));
		if (blockers.ReadBit(PosToIndex(sourceFile, rank))) break;
	}

	for (file = sourceFile + 1; file <= BOARD_DIM - 2; ++file)
	{
		attacks.SetBit(PosToIndex(file, sourceRank));
		if (blockers.ReadBit(PosToIndex(file, sourceRank))) break;
	}

	for (file = sourceFile - 1; file > 0; --file)
	{
		attacks.SetBit(PosToIndex(file, sourceRank));
		if (blockers.ReadBit(PosToIndex(file, sourceRank))) break;
	}

	return attacks;
}

// for a given index, load the binary defined lsb from the provided bitboard (ex: index = 11010 means lod the 2nd, 4th, and 5th lsbs in the board)
BitBoard Board::SetOccupancy(size_t index, BitBoard attackMask) const
{
	BitBoard result;
	for (size_t i = 0; i < attackMask.CountBits(); ++i)
	{
		size_t square = attackMask.GetLSBIndex();
		attackMask.ClearBit(square);
		if (index & (1ULL << i)) result.SetBit(square);
	}

	return result;
}

bool Board::InCheck(Colour colour) const
{
	return false;
}

void Board::DoMove(size_t pieceColour, size_t from, size_t to)
{
	mBitBoards[pieceColour].ClearBit(from).SetBit(to);
}

std::vector<size_t> Board::GetValidLocations(size_t from)
{
	std::vector<size_t> validLocations;
	int pieceColour = GetPieceColourAt(from);
	if (pieceColour == -1) return validLocations;

	Piece piece = static_cast<Piece>(pieceColour & PIECE_MASK);
	Colour colour = static_cast<Colour>(pieceColour & COLOUR_MASK);
	switch (piece)
	{
		case PAWN: validLocations = GetPawnMoves(colour, from); break;
		case KNIGHT: validLocations = GetKnightMoves(colour, from); break;
		//case BISHOP :
		//case ROOK :
		//case QUEEN: validLocations = GetSlidingMoves(); break;
		case KING: validLocations = GetKingMoves(colour, from); break;
	}

	validLocations.erase(std::remove_if(validLocations.begin(), validLocations.end(),
		[this, pieceColour, from](const auto& location)
		{
			DoMove(pieceColour, from, location);
			bool inCheck = InCheck(static_cast<Colour>(pieceColour & COLOUR_MASK));
			DoMove(pieceColour, location, from);
			return inCheck;
		}), validLocations.end());

	return validLocations;
}

bool Board::CheckValidLocation(Colour colour, size_t to) const
{
	for (size_t bitBoard = colour | PAWN; bitBoard < (colour | KING); ++bitBoard)
	{
		if (mBitBoards[bitBoard].ReadBit(to)) return false;
	}

	return true;
}

int Board::GetPieceColourAt(size_t loc) const
{
	int pieceColour = -1;
	for (size_t piece = PAWN; piece <= KING && pieceColour < 0; ++piece)
	{
		for (size_t colour = WHITE; colour <= BLACK && pieceColour < 0; colour += BLACK)
		{
			if (mBitBoards[colour | piece].ReadBit(loc))
			{
				pieceColour = colour | piece;
				break;
			}
		}
	}
	
	return pieceColour;
}

bool Board::TryMove(size_t from, size_t to)
{
	std::vector<size_t> validLocations = GetValidLocations(from);
	currValidLocations.clear();
	if (!validLocations.size() || std::find(validLocations.begin(), validLocations.end(), to) == validLocations.end()) return false;

	int fromPC = GetPieceColourAt(from);
	int toPC = GetPieceColourAt(to);

	DoMove(fromPC, from, to);

	if (toPC >= 0) mBitBoards[toPC].ClearBit(to);

	return true;
}

BitBoard Board::GetColourBoard(Colour colour) const
{
	BitBoard result = mBitBoards[colour | PAWN];
	for (size_t piece = KNIGHT; piece <= KING; ++piece)
	{
		result |= mBitBoards[colour | piece];
	}

	return result;
}

BitBoard Board::GetOccupancyBoard() const
{
	return GetColourBoard(WHITE) | GetColourBoard(BLACK);
}

std::vector<size_t> Board::GetPawnMoves(Colour colour, size_t from) const
{
	std::vector<size_t> validLocations;
	int fromX = from % BOARD_DIM;
	int fromY = from / BOARD_DIM;

	int dir = colour == WHITE ? 1 : -1;
	if (fromY + dir >= BOARD_DIM || fromY + dir < 0) return validLocations;

	BitBoard allPieces = GetOccupancyBoard();
	BitBoard otherPieces = GetColourBoard(static_cast<Colour>(colour ^ COLOUR_MASK));
	bool shovingPiece = allPieces.ReadBit(PosToIndex(fromX, fromY + dir));
	for (int offset = -1; offset <= 1; ++offset)
	{
		bool outsideBoard = fromX + offset < 0 || fromX + offset >= BOARD_DIM;
		bool invalidAttack = !otherPieces.ReadBit(PosToIndex(fromX + offset, fromY + dir));
		if (outsideBoard || (!offset && shovingPiece)) continue;

		validLocations.emplace_back(PosToIndex(fromX + offset, fromY + dir));
	}

	if(!shovingPiece && ((colour == WHITE && fromY == 1) || (colour == BLACK && fromY == BOARD_DIM - 2))) validLocations.emplace_back(PosToIndex(fromX, fromY + 2 * dir));

	return validLocations;
}

std::vector<size_t> Board::GetKnightMoves(Colour colour, size_t from) const
{
	std::vector<size_t> validLocations;
	int fromX = from % BOARD_DIM;
	int fromY = from / BOARD_DIM;
	BitBoard samePieces = GetColourBoard(colour);

	for (int x = -2; x <= 2; ++x)
	{
		for (int y = -2; y <= 2; ++y)
		{
			int toX = fromX + x;
			int toY = fromY + y;
			bool insideBoard = toX >= 0 && toX < BOARD_DIM&& toY >= 0 && toY < BOARD_DIM;
			bool isLMove = abs(fromX - toX) == 2 && abs(fromY - toY) == 1 || abs(fromX - toX) == 1 && abs(fromY - toY) == 2;
			bool invalidAttack = samePieces.ReadBit(PosToIndex(toX, toY));
			if (!insideBoard || !isLMove || invalidAttack) continue;

			validLocations.emplace_back(PosToIndex(toX, toY));
		}
	}

	return validLocations;
}

std::vector<size_t> Board::GetKingMoves(Colour colour, size_t from) const
{
	std::vector<size_t> validLocations;
	int fromX = from % BOARD_DIM;
	int fromY = from / BOARD_DIM;
	BitBoard samePieces = GetColourBoard(colour);

	for (int x = -1; x <= 1; ++x)
	{
		for (int y = -1; y <= 1; ++y)
		{
			if (!x && !y) continue;
			int toX = fromX + x;
			int toY = fromY + y;
			bool insideBoard = toX >= 0 && toX < BOARD_DIM && toY >= 0 && toY < BOARD_DIM;
			bool invalidAttack = samePieces.ReadBit(PosToIndex(toX, toY));
			if (!insideBoard || invalidAttack) continue;
				
			validLocations.emplace_back(PosToIndex(toX, toY));
		}
	}

	return validLocations;
}

size_t Board::PosToIndex(size_t x, size_t y)
{
	return x + BOARD_DIM * y;
}
