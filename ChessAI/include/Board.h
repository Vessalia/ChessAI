#pragma once
#include <array>
#include <vector>

#include <SDL.h>

#include "Core.h"
#include "BitBoard.h"
#include "Texture.h"

constexpr static size_t NUM_BITBOARDS = 16;

enum Piece
{
	PAWN,
	KNIGHT,
	BISHOP,
	ROOK,
	QUEEN,
	KING
};

enum Colour
{
	WHITE = 0,
	BLACK = 8
};

class Board
{
public:
	Board();
	~Board();

	void InitSprites(SDL_Renderer* renderer);

	void Draw(SDL_Renderer* renderer, size_t selectedIndex);
	void Resize(int width, int height);

private:
	friend class Game;

	static constexpr uint8_t PIECE_MASK = 0b00000111;
	static constexpr uint8_t COLOUR_MASK = 0b00001000;
	
	static constexpr size_t INVALID_INDEX = BOARD_DIM * BOARD_DIM;

	size_t mWidth = SCREEN_WIDTH / BOARD_DIM;
	size_t mHeight = SCREEN_HEIGHT / BOARD_DIM;

	std::array<BitBoard, NUM_BITBOARDS> mBitBoards;
	std::array<Texture*, NUM_BITBOARDS> mPieceSprites;

	bool InCheck(Colour colour) const;

	void DoMove(size_t pieceColour, size_t from, size_t to);

	std::vector<size_t> GetValidLocations(size_t from);
	bool CheckValidLocation(Colour colour, size_t to) const;
	int GetPieceColourAt(size_t loc) const;

	bool TryMove(size_t from, size_t to);

	std::vector<size_t> GetPawnMoves(Colour colour, size_t from) const;
	std::vector<size_t> GetKnightMoves(Colour colour, size_t from) const;
	std::vector<size_t> GetKingMoves(Colour colour, size_t from) const;

	BitBoard GetColourBoard(Colour colour) const;
	BitBoard GetOccupancyBoard() const;

	static size_t PosToIndex(size_t x, size_t y);
};
