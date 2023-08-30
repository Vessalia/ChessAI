#pragma once
#include <memory>
#include <string>
#include <SDL_image.h>

class Texture
{
public:
	Texture();
	Texture(const std::string& path, SDL_Renderer* renderer);

	~Texture();

	void RenderTexture(SDL_Renderer* renderer, int x, int y, SDL_Rect* clip = nullptr, double angle = 0.0, SDL_Point* center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE) const;

	void SetWidth(size_t width)
	{
		mWidth = width;
	}

	void SetHeight(size_t height)
	{
		mHeight = height;
	}

private:
	SDL_Texture* mTex;

	size_t mWidth;
	size_t mHeight;
};
