#pragma once
#include "manager.h"

struct Label
{
	std::string text = "";
	std::string fontID = "";
	SDL_Texture* tex = nullptr;
	SDL_Rect pos{ 0, 0, 0, 0 };
};