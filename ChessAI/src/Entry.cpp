#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_image.h>

#include "Core.h"
#include "Game.h"

static SDL_Window* gWindow = nullptr;
static SDL_Renderer* gRenderer = nullptr;

static Game game;

bool init();
void handleMousePress();
void draw();
void close();

int main()
{
    FATAL_ASSERT(init() && "Failed to initialize!");

    bool lastPressed = false;
    bool quit = false;
    SDL_Event e;

    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN && !lastPressed)
            {
                if (e.button.button == SDL_BUTTON_LEFT)
                {
                    lastPressed = true;
                    handleMousePress();
                }
            }
            else if (e.type == SDL_MOUSEBUTTONUP)
            {
                if (e.button.button == SDL_BUTTON_LEFT)
                {
                    lastPressed = false;
                }
            }
        }
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer);

        draw();

        SDL_RenderPresent(gRenderer);
    }

    close();

    return 0;
}

bool init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL could not initialize. SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }

    gWindow = SDL_CreateWindow("Chess", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!gWindow)
    {
        std::cout << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_SOFTWARE);
    if (!gRenderer)
    {
        std::cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

    game.InitSprites(gRenderer);

    return true;
}

void handleMousePress()
{
    int x, y;
    SDL_GetMouseState(&x, &y);
    game.HandleMousePress(x, SCREEN_HEIGHT - y);
}

void draw()
{
    game.Draw(gRenderer);
}

void close()
{
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gRenderer = nullptr;
    gWindow = nullptr;

    IMG_Quit();
    SDL_Quit();
}
