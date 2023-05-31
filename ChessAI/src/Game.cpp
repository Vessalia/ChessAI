#include "Game.h"

Game::Game() : mBoard(Board()) { }

void Game::InitSprites(SDL_Renderer* renderer)
{
	mBoard.InitSprites(renderer);
}

void Game::Draw(SDL_Renderer* renderer) const
{
	mBoard.Draw(renderer);
}
