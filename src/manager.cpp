#include "manager.h"
#include "labels.h"
#include <iostream>

Manager::Manager(SDL_Renderer* renderer) : renderer(renderer)
{
	std::cout << "Created the manager\n";
}

Manager::~Manager()
{
	for (const auto& tex : textures)
	{
		std::cout << "Destroying texture: " << tex.first << "\n";
		SDL_DestroyTexture(tex.second);
	}

	for (const auto& label : labels)
	{
		if (!label.second->tex)
		{
			std::cout << "Couldn't get a texture from label " << label.first << std::endl;
			continue;
		}

		std::cout << "Destroying text label: " << label.first << "\n";
		SDL_DestroyTexture(label.second->tex);
		delete label.second;
	}

	for (const auto& font : fonts)
	{
		std::cout << "Destroying font: " << font.first << "\n";
		TTF_CloseFont(font.second);
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

void Manager::DrawTexture(const char* textureID, SDL_Rect* src, SDL_Rect* dest, double angle)
{
	std::string s_textureID = textureID;
	auto it = textures[s_textureID];

	if (!it)
	{
		std::cout << "Texture \"" << textureID << "\" doesn't exist";
		return;
	}

	SDL_RenderCopyEx(renderer, it, src, dest, angle, NULL, SDL_FLIP_NONE);
}

void Manager::DrawTexture(contents textureID, SDL_Rect* src, SDL_Rect* dest, double angle)
{
	auto it = textures[std::to_string(textureID)];

	if (!it)
	{
		std::cout << "Texture \"" << textureID << "\" doesn't exist";
		return;
	}

	SDL_RenderCopyEx(renderer, it, src, dest, angle, NULL, SDL_FLIP_NONE);
}

void Manager::LoadFont(std::string fontID, const char* path, uint16_t fontSize)
{
	fonts.emplace(fontID, TTF_OpenFont(path, fontSize));
}

void Manager::CreateLabel(std::string fontID, std::string text, uint32_t x, uint32_t y, SDL_Color* color, std::string labelID)
{
	Label* label = new Label();
	label->fontID = fontID;
	label->text = text;
	label->pos.x = x;
	label->pos.y = y;
	SDL_Surface* surface = TTF_RenderText_Blended(fonts[fontID], text.c_str(), *color);
	label->tex = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	labels.emplace(labelID, label);

	SDL_QueryTexture(label->tex , nullptr, nullptr, &label->pos.w, &label->pos.h);
}

void Manager::UpdateLabelText(std::string text, std::string labelID, SDL_Color* color)
{
	SDL_DestroyTexture(labels[labelID]->tex);

	labels[labelID]->text = text;
	SDL_Surface* surface = TTF_RenderText_Blended(fonts[labels[labelID]->fontID], text.c_str(), *color);
	labels[labelID]->tex = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	SDL_QueryTexture(labels[labelID]->tex, nullptr, nullptr, &labels[labelID]->pos.w, &labels[labelID]->pos.h);
}

void Manager::UpdateLabelPos(std::string labelID, uint32_t x, uint32_t y)
{
	labels[labelID]->pos.x = x;
	labels[labelID]->pos.y = y;
}

SDL_Rect* Manager::GetLabelRect(std::string labelID)
{
	return &labels[labelID]->pos;
}

void Manager::DrawLabel(std::string labelID)
{
	SDL_RenderCopy(renderer, labels[labelID]->tex, nullptr, &labels[labelID]->pos);
}

void Manager::Log()
{
	std::cout << "\nLoaded textures:" << std::endl;

	for (const auto& texture : textures)
	{
		std::cout << texture.first << "\n";
	}

	std::cout << "\nLoaded fonts:" << std::endl;

	for (const auto& font : fonts)
	{
		std::cout << font.first << "\n";
	}

	std::cout << "\nCreated labels:" << std::endl;
	
	for (const auto& label : labels)
	{
		std::cout << label.first << "\n";
	}
}