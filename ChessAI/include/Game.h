#pragma once
#include "Board.h"
#include <SDL.h>

class Game
{
public:
	Game();

	void InitSprites(SDL_Renderer* renderer);

	void Draw(SDL_Renderer* renderer) const;

private:
	Board mBoard;
};
