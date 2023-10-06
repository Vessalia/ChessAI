#pragma once
#include <array>
#include <vector>

#include <SDL.h>

#include "Core.h"
#include "BoardCore.h"
#include "BitBoard.h"
#include "Texture.h"

void InitSprites(SDL_Renderer* renderer);
void DestroySprites();

void ResizeSprites();

class Board
{
public:
	Board();
	Board(const std::string& fen);

	void Draw(SDL_Renderer* renderer, size_t selectedIndex);

	void Print() const;

	static Board ParseFen(const std::string& fen);

private:
	friend class Game;

	static constexpr uint8_t PIECE_MASK = 0b00000111;
	static constexpr uint8_t COLOUR_MASK = 0b00001000;
	
	static constexpr size_t INVALID_INDEX = BOARD_DIM * BOARD_DIM;

	std::array<BitBoard, NUM_BITBOARDS> mPieceBoards;

	std::array<std::array<BitBoard, BOARD_DIM * BOARD_DIM>, NUM_COLOURS> mPawnAttacks;
	std::array<BitBoard, BOARD_DIM * BOARD_DIM> mKnightAttacks;
	std::array<BitBoard, BOARD_DIM * BOARD_DIM> mKingAttacks;

	std::array<BitBoard, BOARD_DIM * BOARD_DIM> mBishopMasks;
	std::array<BitBoard, BOARD_DIM * BOARD_DIM> mRookMasks;
	std::array<std::vector<BitBoard>, BOARD_DIM * BOARD_DIM> mBishopAttacks;
	std::array<std::vector<BitBoard>, BOARD_DIM * BOARD_DIM> mRookAttacks;

	BitBoard GetBishopAttacks(size_t square, BitBoard occupancy) const;
	BitBoard GetRookAttacks(size_t square, BitBoard occupancy) const;
	BitBoard GetQueenAttacks(size_t square, BitBoard occupancy) const;

	bool IsSquareAttacked(size_t square, Colour side) const;

	void GenerateMoves() const;

	bool InCheck(Colour colour) const;

	void DoMove(size_t pieceColour, size_t from, size_t to);
	int GetPieceColourAt(size_t loc) const;

	BitBoard GetColourBoard(Colour colour) const;
	BitBoard GetOccupancyBoard() const;
};
