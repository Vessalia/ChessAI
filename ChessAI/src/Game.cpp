#include "Game.h"
#include "Core.h"
#include "BoardCore.h"
#include <random>

Game::Game()
	: mSelectedIndex(Board::INVALID_INDEX) {}

void Game::Draw(SDL_Renderer* renderer)
{
	mBoard.Draw(renderer, mSelectedIndex);
}

void Game::Resize(int width, int height)
{
	mBoard.Resize(width, height);
}

void Game::HandleMousePress(int x, int y)
{
	if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) return;

	x /= texWidth;
	y /= texHeight;
	if (!TryMove(x, y))
	{
		TrySelect(x, y);
	}
}

void Game::Print()
{
	mBoard.Print();
}

bool Game::TrySelect(size_t x, size_t y)
{
	size_t index = PosToIndex(x, y);
	if (index >= Board::INVALID_INDEX || mBoard.GetPieceColourAt(index) < 0) return false;
	mSelectedIndex = index;
	return true;
}

bool Game::TryMove(size_t x, size_t y)
{
	if (mSelectedIndex >= Board::INVALID_INDEX) return false;
	bool success = false; //mBoard.TryMove(mSelectedIndex, PosToIndex(x, y));
	mSelectedIndex = Board::INVALID_INDEX;
	return success;
}
