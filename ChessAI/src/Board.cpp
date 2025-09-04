#include "Board.h"
#include <algorithm>

static std::array<Texture*, NUM_BITBOARDS> pieceSprites;

static uint8_t WHITE_KING_SIDE_CASTLE = 0b00001000;
static uint8_t WHITE_QUEEN_SIDE_CASTLE = 0b00000100;
static uint8_t BLACK_KING_SIDE_CASTLE = 0b00000010;
static uint8_t BLACK_QUEEN_SIDE_CASTLE = 0b00000001;

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
	for (size_t colour = WHITE; colour <= BLACK; colour += BLACK)
	{
		for (size_t piece = PAWN; piece <= KING; ++piece)
		{
			pieceSprites[colour | piece]->SetWidth(TEX_WIDTH);
			pieceSprites[colour | piece]->SetHeight(TEX_HEIGHT);
		}
	}
}

Board::Board()
{
	for (size_t row = 0; row < BOARD_DIM; ++row)
	{
		for (size_t col = 0; col < BOARD_DIM; ++col) // should probably switch to doing inside out
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

	GenerateMoves();
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
	GetOccupancyBoard().Print();
	PrintMoveList();
}

Colour sideToMove = WHITE;
uint8_t castlingRights = 0b00001111;
size_t enpassant;
Board Board::ParseFen(const std::string& fen)
{
	Board parsedBoard;

	for (size_t piece = PAWN; piece <= KING; ++piece)
	{
		for (size_t colour = WHITE; colour <= BLACK; colour += BLACK)
		{
			parsedBoard.mPieceBoards[colour | piece] = BitBoard();
		}
	}

	std::vector<std::string> boardState = Tokenize(fen, " ");
	size_t boardIndex = (BOARD_DIM - 1) * BOARD_DIM;
	for (char curr : boardState[0])
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
		else if (curr == '/')
		{
			boardIndex -= 2 * BOARD_DIM;
		}
	}

	boardState.erase(boardState.begin());
	if (boardState.size() == 0) return parsedBoard;

	sideToMove = boardState[0] == "w" ? WHITE : BLACK;

	boardState.erase(boardState.begin());
	if (boardState.size() == 0) return parsedBoard;

	castlingRights =         0b00000000;
	for (char castle : boardState[0])
	{
		switch (castle)
		{
			case 'K': castlingRights |= WHITE_KING_SIDE_CASTLE; break;
			case 'Q': castlingRights |= WHITE_QUEEN_SIDE_CASTLE; break;
			case 'k': castlingRights |= BLACK_KING_SIDE_CASTLE; break;
			case 'q': castlingRights |= BLACK_QUEEN_SIDE_CASTLE; break;
			default: break;
		}
	}

	boardState.erase(boardState.begin());
	if (boardState.size() == 0) return parsedBoard;

	enpassant = Board::INVALID_INDEX;
	int col = boardState[0][0] - 'a';
	int row = boardState[0][1] - '1';
	if (boardState[0].size() == 2 &&
	    col < BOARD_DIM && col >= 0 &&
	    row < BOARD_DIM && row >= 0)
	{
		enpassant = PosToIndex(col, row);
	}

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

bool Board::IsSquareAttacked(size_t square, Colour side) const
{
	if (mPawnAttacks[side ? 0 : 1][square] & mPieceBoards[side | PAWN]) return true;
	if (mKnightAttacks[square] & mPieceBoards[side | KNIGHT]) return true;
	if (mKingAttacks[square] & mPieceBoards[side | KING]) return true;
	if (GetBishopAttacks(square, GetOccupancyBoard()) & mPieceBoards[side | BISHOP]) return true;
	if (GetRookAttacks(square, GetOccupancyBoard()) & mPieceBoards[side | ROOK]) return true;
	if (GetQueenAttacks(square, GetOccupancyBoard()) & mPieceBoards[side | QUEEN]) return true;

	return false;
}

void Board::GenerateMoves()
{
	size_t sourceSquare;

	Colour colour = sideToMove;

	BitBoard state, attacks, selfOccupancy, otherOccupancy, occupancy;
	selfOccupancy = GetColourBoard(colour);
	otherOccupancy = GetColourBoard((Colour)(((uint8_t)colour) ^ BLACK));
	occupancy = selfOccupancy | otherOccupancy;

	uint8_t kingSideCastle = WHITE_KING_SIDE_CASTLE | BLACK_KING_SIDE_CASTLE;
	uint8_t queenSideCastle = WHITE_QUEEN_SIDE_CASTLE | BLACK_QUEEN_SIDE_CASTLE;
	size_t boardSide = sideToMove == WHITE ? 0 : BOARD_DIM - 1;

	if (castlingRights & kingSideCastle)
	{
		if (!occupancy.ReadBit(PosToIndex(BOARD_DIM - 3, boardSide)) &&
			!occupancy.ReadBit(PosToIndex(BOARD_DIM - 2, boardSide)))
		{
			std::cout << "kingside castle " << std::bitset<4>(castlingRights) << std::endl;
			//AddMove();
		}
	}

	if (castlingRights & queenSideCastle)
	{
		if (!occupancy.ReadBit(PosToIndex(1, boardSide)) &&
			!occupancy.ReadBit(PosToIndex(2, boardSide)) &&
			!occupancy.ReadBit(PosToIndex(3, boardSide)))
		{
			std::cout << "queenside castle " << std::bitset<4>(castlingRights) << std::endl;
			//AddMove();
		}
	}

	for (size_t pieceVal = PAWN; pieceVal <= KING; ++pieceVal)
	{
		Piece piece = (Piece)pieceVal;
		BitBoard state = mPieceBoards[colour | piece];
		while (state)
		{
			sourceSquare = state.GetLSBIndex();
			BitBoard attacks;

			switch (piece)
			{
			case PAWN:
				GeneratePawnMoves(colour, sourceSquare, occupancy, otherOccupancy);
				break;
			case KNIGHT:
				attacks = mKnightAttacks[sourceSquare] & (~selfOccupancy);
				GenerateMovesForPiece(piece, colour, sourceSquare, attacks, otherOccupancy);
				break;
			case BISHOP:
				attacks = GetBishopAttacks(sourceSquare, occupancy) & (~selfOccupancy);
				GenerateMovesForPiece(piece, colour, sourceSquare, attacks, otherOccupancy);
				break;
			case ROOK:
				attacks = GetRookAttacks(sourceSquare, occupancy) & (~selfOccupancy);
				GenerateMovesForPiece(piece, colour, sourceSquare, attacks, otherOccupancy);
				break;
			case QUEEN:
				attacks = GetQueenAttacks(sourceSquare, occupancy) & (~selfOccupancy);
				GenerateMovesForPiece(piece, colour, sourceSquare, attacks, otherOccupancy);
				break;
			case KING:
				attacks = mKingAttacks[sourceSquare] & (~selfOccupancy);
				GenerateMovesForPiece(piece, colour, sourceSquare, attacks, otherOccupancy);
				break;
			}

			state.PopBit(sourceSquare);
		}
	}
}

void Board::GeneratePawnMoves(Colour colour, size_t sourceSquare, const BitBoard& occupancy, const BitBoard& otherOccupancy)
{
	int dir = (colour == WHITE) ? 1 : -1;
	size_t targetSquare = sourceSquare + dir * BOARD_DIM;

	bool promotionRow = sourceSquare >= (BOARD_DIM - 2) * BOARD_DIM && sourceSquare <= (BOARD_DIM - 1) * BOARD_DIM;
	bool doublePushRow = sourceSquare >= BOARD_DIM && sourceSquare < 2 * BOARD_DIM;

	if (colour == BLACK)
	{
		std::swap(promotionRow, doublePushRow);
	}

	// quiet pawn moves
	if (targetSquare >= 0 && targetSquare < BOARD_DIM * BOARD_DIM && !occupancy.ReadBit(targetSquare))
	{
		if (promotionRow)
		{
			std::cout << "promote " << sourceSquare << "->" << targetSquare << std::endl;
			AddMove(sourceSquare, targetSquare, colour | PAWN, KNIGHT, false, false, false, false);
			AddMove(sourceSquare, targetSquare, colour | PAWN, BISHOP, false, false, false, false);
			AddMove(sourceSquare, targetSquare, colour | PAWN, ROOK, false, false, false, false);
			AddMove(sourceSquare, targetSquare, colour | PAWN, QUEEN, false, false, false, false);
		}
		else
		{
			std::cout << "push " << sourceSquare << "->" << targetSquare << std::endl;
			AddMove(sourceSquare, targetSquare, colour | PAWN, 0, false, false, false, false);

			if (doublePushRow && !occupancy.ReadBit(targetSquare + dir * BOARD_DIM))
			{
				std::cout << "double push " << sourceSquare << "->" << targetSquare + dir * BOARD_DIM << std::endl;
				AddMove(sourceSquare, targetSquare + dir * BOARD_DIM, colour | PAWN, 0, false, true, false, false);
			}
		}
	}

	// pawn attack moves
	int occIndex = colour != WHITE;
	BitBoard attacks = mPawnAttacks[occIndex][sourceSquare] & otherOccupancy;

	while (attacks)
	{
		targetSquare = attacks.GetLSBIndex();

		if (promotionRow)
		{
			std::cout << "capture promote " << sourceSquare << "->" << targetSquare << std::endl;
			AddMove(sourceSquare, targetSquare, colour | PAWN, KNIGHT, true, false, false, false);
			AddMove(sourceSquare, targetSquare, colour | PAWN, BISHOP, true, false, false, false);
			AddMove(sourceSquare, targetSquare, colour | PAWN, ROOK, true, false, false, false);
			AddMove(sourceSquare, targetSquare, colour | PAWN, QUEEN, true, false, false, false);
		}
		else
		{
			std::cout << "capture " << sourceSquare << "->" << targetSquare << std::endl;
			AddMove(sourceSquare, targetSquare, colour | PAWN, 0, true, false, false, false);
		}

		attacks.PopBit(targetSquare);
	}

	if (enpassant != INVALID_INDEX && colour == sideToMove)
	{
		BitBoard enpassantAttacks = mPawnAttacks[occIndex][sourceSquare] & BitBoard().SetBit(enpassant);
		if (enpassantAttacks)
		{
			targetSquare = enpassantAttacks.GetLSBIndex();
			std::cout << "capture enpassant " << sourceSquare << "->" << targetSquare << std::endl;
		}
	}
}

void Board::GenerateMovesForPiece(Piece piece, Colour colour, size_t sourceSquare, BitBoard& attacks, const BitBoard& occupancy)
{
	while (attacks)
	{
		size_t targetSquare = attacks.GetLSBIndex();

		if (occupancy.ReadBit(targetSquare))
		{
			std::cout << "piece capture: " << sourceSquare << "->" << targetSquare << std::endl;
			AddMove(sourceSquare, targetSquare, colour | piece, 0, true, false, false, false);
		}
		else
		{
			std::cout << "piece move: " << sourceSquare << "->" << targetSquare << std::endl;
			AddMove(sourceSquare, targetSquare, colour | piece, 0, false, false, false, false);
		}

		attacks.PopBit(targetSquare);
	}
}

void Board::AddMove(uint64_t sourceSquare, uint64_t targetSquare, uint8_t piece, uint8_t promoted, bool capture, bool doublePush, bool enpassant, bool castling)
{
	if (mMoveCount < mMoveList.size())
	{
		mMoveList[mMoveCount++] = EncodeMove(sourceSquare, targetSquare, piece, promoted, capture, doublePush, enpassant, castling);
	}
}

void Board::PrintMoveList() const
{
	for (unsigned int i = 0; i < mMoveCount; ++i)
	{
		PrintMove(mMoveList[i]);
	}
}

void Board::PrintMove(int moveEncoding) const
{
	char source = DecodeMoveSource(moveEncoding);
	char target = DecodeMoveTarget(moveEncoding);
	char piece = DecodeMovePiece(moveEncoding);
	char promote = DecodeMovePromote(moveEncoding);
	char flags = DecodeMoveFlags(moveEncoding);

	char name = WHITE | PAWN;
	char promoteName = WHITE | PAWN;
	for (auto kvp : fenHelper)
	{
		if (piece == kvp.second)
		{
			name = kvp.first;
		}
		if ((BLACK | promote) == kvp.second)
		{
			promoteName = kvp.first;
		}
	}

	std::cout << indexToCoord[source] << indexToCoord[target];
	if ((promote & ~piece) & 0b111) std::cout << promoteName;
	std::cout << std::endl;
}

bool Board::InCheck(Colour colour) const
{
	size_t kingSquare = mPieceBoards[colour | KING].GetLSBIndex();
	return IsSquareAttacked(kingSquare, Colour(colour ^ BLACK));
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
