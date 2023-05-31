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

	void RenderTexture(SDL_Renderer* renderer, int x, int y) const;
	void RenderTextureOptions(SDL_Renderer* renderer, int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip) const;

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
