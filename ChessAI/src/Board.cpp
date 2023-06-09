#include "Board.h"
#include "Core.h"

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

			if (row < 2 || row >= BOARD_DIM - 2) mBitBoards[index].SetBit(col, row);
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
			bool inCheck = InCheck(pieceColour & COLOUR_MASK);
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
		bool invalidAttack = offset && !otherPieces.ReadBit(PosToIndex(fromX + offset, fromY + dir));
		if (outsideBoard || (!offset && shovingPiece) || invalidAttack) continue;

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
