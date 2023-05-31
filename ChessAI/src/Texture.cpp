#pragma once
#include "Texture.h"
#include <iostream>

Texture::Texture() 
	: mTex(nullptr)
	, mWidth(0)
	, mHeight(0) { }

Texture::Texture(const std::string& path, SDL_Renderer* renderer) : Texture()
{
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (!loadedSurface)
    {
        std::cout << "Unable to load image " << path << "! SDL_image Error : " << IMG_GetError() << std::endl;
        return;
    }

    SDL_Texture* newTex = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if (!newTex)
    {
        std::cout << "Unable to create texture from " << path << "! SDL Error : " << SDL_GetError() << std::endl;
        SDL_FreeSurface(loadedSurface);
        return;
    }

    mWidth = loadedSurface->w;
    mHeight = loadedSurface->h;
    mTex = newTex;

    SDL_FreeSurface(loadedSurface);
}

Texture::~Texture()
{
    if(mTex) SDL_DestroyTexture(mTex);
}

void Texture::RenderTexture(SDL_Renderer* renderer, int x, int y) const
{
    RenderTextureOptions(renderer, x, y, nullptr, 0.0, nullptr, SDL_FLIP_NONE);
}

void Texture::RenderTextureOptions(SDL_Renderer* renderer, int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip) const
{
    SDL_Rect renderQuad = { x, y, mWidth, mHeight };

    if (clip)
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    SDL_RenderCopyEx(renderer, mTex, clip, &renderQuad, angle, center, flip);
}
