#pragma once
#include "Board.h"
#include <SDL.h>

class Game
{
public:
	Game();

	void InitSprites(SDL_Renderer* renderer);

	void Draw(SDL_Renderer* renderer);
	void Resize(int width, int height);
	void HandleMousePress(int x, int y);

	void Print();

private:
	Board mBoard;

	size_t mSelectedIndex; // could be delegated to separate class, but this is easier

	bool TrySelect(size_t x, size_t y);
	bool TryMove(size_t x, size_t y);
};
