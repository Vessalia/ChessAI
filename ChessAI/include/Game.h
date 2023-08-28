#pragma once
#include "Board.h"

class Game
{
public:
	Game();

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
