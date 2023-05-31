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

#define WIDTH (SCREEN_WIDTH / BOARD_DIM)
#define HEIGHT (SCREEN_HEIGHT / BOARD_DIM)
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
			mPieceSprites[colour | piece]->SetWidth(WIDTH);
			mPieceSprites[colour | piece]->SetHeight(HEIGHT);
		}
	}
}

void Board::Draw(SDL_Renderer* renderer) const
{
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

			SDL_Rect square = { x * WIDTH, HEIGHT * (BOARD_DIM - 1) - y * HEIGHT, WIDTH, HEIGHT };

			SDL_SetRenderDrawColor(renderer, colourR, colourG, colourB, colourA);
			SDL_RenderFillRect(renderer, &square);

			int pieceColour = GetPieceAt(Board::PosToIndex(x, y));
			if (pieceColour == -1) continue;

			mPieceSprites[pieceColour]->RenderTexture(renderer, x * WIDTH, HEIGHT * (BOARD_DIM - 1) - y * HEIGHT);
		}
	}
}

void Board::DoMove(size_t pieceColour, size_t from, size_t to)
{
	mBitBoards[pieceColour].ClearBit(from).SetBit(to);
}

std::vector<size_t> GetPawnMoves(Colour colour, size_t from)
{
	std::vector<size_t> validLocations;
	int fromX = from % BOARD_DIM;
	int fromY = from / BOARD_DIM;

	int dir = colour == WHITE ? 1 : -1;
	if (fromY + dir >= BOARD_DIM || fromY + dir < 0) return validLocations;

	for (int offset = -1; offset <= 1; ++offset)
	{
		if (fromX + offset < 0 || fromX + offset >= BOARD_DIM) continue;
		validLocations.emplace_back(Board::PosToIndex(fromX + offset, fromY + dir));
	}

	return validLocations;
}

std::vector<size_t> GetKnightMoves(size_t from)
{
	std::vector<size_t> validLocations;
	int fromX = from % BOARD_DIM;
	int fromY = from / BOARD_DIM;

	for (int x = -2; x <= 2; ++x)
	{
		for (int y = -2; y <= 2; ++y)
		{
			int toX = fromX + x;
			int toY = fromY + y;
			bool insideBoard = toX >= 0 && toX < BOARD_DIM&& toY >= 0 && toY < BOARD_DIM;
			bool isLMove = abs(fromX - toX) == 2 && abs(fromY - toY) == 1 || abs(fromX - toX) == 1 && abs(fromY - toY) == 2;
			if (insideBoard && isLMove)
			{
				validLocations.emplace_back(Board::PosToIndex(toX, toY));
			}
		}
	}

	return validLocations;
}

std::vector<size_t> GetKingMoves(size_t from)
{
	std::vector<size_t> validLocations;
	int fromX = from % BOARD_DIM;
	int fromY = from / BOARD_DIM;

	for (int x = -1; x <= 1; ++x)
	{
		for (int y = -1; y <= 1; ++y)
		{
			bool insideBoard = fromX + x >= 0 && fromX + x < BOARD_DIM && fromY + y >= 0 && fromY + y < BOARD_DIM;
			if (insideBoard) validLocations.emplace_back(Board::PosToIndex(fromX + x, fromY + y));
		}
	}

	return validLocations;
}

std::vector<size_t> Board::GetValidLocations(size_t from) const
{
	std::vector<size_t> validLocations;
	int pieceColour = GetPieceAt(from);
	if (pieceColour == -1) return validLocations;

	Piece piece = static_cast<Piece>(pieceColour & Board::PIECE_MASK);
	Colour colour = static_cast<Colour>(pieceColour & Board::COLOUR_MASK);

	switch (piece)
	{
		case PAWN : validLocations = GetPawnMoves(colour, from);
		case KNIGHT : validLocations = GetKnightMoves(from);
		//case BISHOP :
		//case ROOK :
		//case QUEEN: validLocations = GetSlidingMoves();
		case KING: validLocations = GetKingMoves(from);
	}

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

int Board::GetPieceAt(size_t loc) const
{
	int pieceColour = -1;
	for (size_t piece = PAWN; piece <= KING; ++piece)
	{
		for (size_t colour = WHITE; colour <= BLACK; colour += BLACK)
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

size_t Board::PosToIndex(size_t x, size_t y)
{
	return x + BOARD_DIM * y;
}
