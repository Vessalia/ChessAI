#include "Board.h"

Board::Board()
{
	for (size_t i = 0; i < mBitBoards.size(); ++i)
	{
		mBitBoards[i] = BitBoard();
	}

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
						case ('a'):
						case ('h'): index |= ROOK; break;
						case ('b'):
						case ('g'): index |= KNIGHT; break;
						case ('c'):
						case ('f'): index |= BISHOP; break;
						case ('d'): index |= QUEEN; break;
						case ('e'): index |= KING; break;
					}
				}
			}

			if(row <= 2 || row >= BOARD_DIM - 1) mBitBoards[index].SetBit(row, col, 1);
		}
	}
}
