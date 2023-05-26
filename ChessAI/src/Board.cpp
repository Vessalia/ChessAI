#include "Board.h"
#include <tuple>

Board::Board()
{
	for (size_t row = 1; row <= BOARD_DIM; ++row)
	{
		for (unsigned char col = 'a'; col < BOARD_DIM + 'a'; ++col)
		{
			unsigned int index = WHITE;
			switch (row)
			{
				case 2: index |= PAWN; break;
				case BOARD_DIM - 1: index = BLACK | PAWN; break;

				case BOARD_DIM : index = BLACK;
				case 1 :
				{
					switch (col)
					{
						case 'a':
						case 'h': index |= ROOK; break;

						case 'b':
						case 'g': index |= KNIGHT; break;

						case 'c':
						case 'f': index |= BISHOP; break;

						case 'd': index |= QUEEN; break;
						case 'e': index |= KING; break;
					}
				}
			}

			if (row <= 2 || row >= BOARD_DIM - 1) mBitBoards[index].SetBit(col, row);
		}
	}
}

void Board::DoMove(size_t pieceColour, size_t from, size_t to)
{
	mBitBoards[pieceColour].ClearBit(from).SetBit(to);
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
		case KNIGHT : validLocations = GetKnightMoves();
		case BISHOP :
		case ROOK :
		case QUEEN: validLocations = GetSlidingMoves();
		case KING: validLocations = GetKingMoves();
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
	for (size_t piece = PAWN; piece < KING; ++piece)
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

std::vector<size_t> Board::GetPawnMoves(Colour colour, size_t from) const
{
	std::vector<size_t> validLocations;

	int dir = colour == WHITE ? BOARD_DIM : -BOARD_DIM;
	if (from + dir >= BOARD_DIM * BOARD_DIM || static_cast<int64_t>(from) + dir < 0) return validLocations;

	for (int offset = -1; offset <= 1; ++offset)
	{
		size_t to = from + dir + offset;
		bool validLocation = CheckValidLocation(colour, to);
		
		if (validLocation)
		{
			validLocations.push_back(to);
		}
	}

	return validLocations;
}
