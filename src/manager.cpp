#include "manager.h"
#include <iostream>

Manager::Manager(SDL_Renderer* renderer) : renderer(renderer)
{
	std::cout << "Created a manager\n";
}

Manager::~Manager()
{
	for (const auto& val : textures)
	{
		std::cout << "Destroying " << val.first <<"\n";
		SDL_DestroyTexture(val.second);
	}

	std::cout << "Destroyed a manager\n";
}

void Manager::LoadTexture(const char* textureID, const char* path)
{
	SDL_Surface* tempSurface = IMG_Load(path);
	if (!tempSurface)
	{
		std::cout << "Failed to load a image: " << SDL_GetError() << "\n";
		return;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
	if (!texture)
	{
		std::cout << "Failed to load a image: " << SDL_GetError() << "\n";
		return;
	}

	SDL_FreeSurface(tempSurface);

	textures.emplace(textureID, texture);

	return;
}

void Manager::Draw(const char* textureID, SDL_Rect dest)
{
	auto it = textures.find(textureID);

	if (it == textures.end())
	{
		std::cout << "Texture \"" << textureID << "\" doesn't exist";
		return;
	}

	SDL_Rect src = { 0, 0, 32, 32 };

	printf("x: %d, y: %d, w: %d, h: %d\n", dest.x, dest.y, dest.w, dest.h);

	SDL_RenderCopyEx(renderer, it->second, &src, &dest, NULL, NULL, SDL_FLIP_NONE);
}

void Manager::Log()
{
	std::cout << "Loaded textures:" << std::endl;

	for (const auto& texture : textures)
	{
		std::cout << texture.first << "\n";
	}
}