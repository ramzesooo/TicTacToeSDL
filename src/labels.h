#pragma once
#include "SDL.h"

struct Label
{
	std::string text = "";
	std::string fontID = "";
	SDL_Texture* tex = nullptr;
	SDL_Rect pos{ 0, 0, 0, 0 };
	SDL_Color* color;
};