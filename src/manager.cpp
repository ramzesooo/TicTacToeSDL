#include "manager.h"
#include <iostream>

Manager::Manager(SDL_Renderer* renderer) : renderer(renderer)
{
	std::cout << "Created the manager\n";
}

Manager::~Manager()
{
	for (const auto& val : textures)
	{
		SDL_DestroyTexture(val.second);
	}

	std::cout << "Destroyed the manager\n";
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
}

void Manager::LoadTexture(contents textureID, const char* path)
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

	textures.emplace(std::to_string(textureID), texture);
}

void Manager::Draw(const char* textureID, SDL_Rect* src, SDL_Rect* dest)
{
	std::string s_textureID = textureID;
	auto it = textures[s_textureID];

	if (!it)
	{
		std::cout << "Texture \"" << textureID << "\" doesn't exist";
		return;
	}

	SDL_RenderCopyEx(renderer, it, src, dest, NULL, NULL, SDL_FLIP_NONE);
}

void Manager::Draw(contents textureID, SDL_Rect* src, SDL_Rect* dest)
{
	auto it = textures[std::to_string(textureID)];

	if (!it)
	{
		std::cout << "Texture \"" << textureID << "\" doesn't exist";
		return;
	}

	SDL_RenderCopyEx(renderer, it, src, dest, NULL, NULL, SDL_FLIP_NONE);
}

void Manager::Log()
{
	std::cout << "Loaded textures:" << std::endl;

	for (const auto& texture : textures)
	{
		std::cout << texture.first << "\n";
	}
}