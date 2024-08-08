#pragma once
#include "app.h"

struct Label;

class Manager
{
public:
	Manager(SDL_Renderer* renderer, App* appPtr);
	~Manager();

	void LoadTexture(const char* textureID, const char* path);
	void LoadTexture(uint32_t textureID, const char* path); // for contents
	void DrawTexture(const char* textureID, SDL_Rect* src, SDL_Rect* dest, double angle = 0);
	void DrawTexture(uint32_t textureID, SDL_Rect* src, SDL_Rect* dest, double angle = 0); // for contents

	void LoadFont(std::string fontID, const char* path, uint16_t fontSize);
	void CreateLabel(std::string fontID, std::string text, uint32_t x, uint32_t y, SDL_Color* color, std::string labelID);

	void UpdateLabelText(std::string text, std::string labelID, SDL_Color* color);
	void UpdateLabelPos(std::string labelID, uint32_t x, uint32_t y);
	void CenterLabelX(std::string labelID);
	void UpdateFont(std::string labelID, std::string newFontID);
	SDL_Rect* GetLabelRect(std::string labelID);

	void DrawLabel(std::string labelID);

	void Log();
private:
	SDL_Renderer* renderer;
	App* app;
	std::unordered_map<std::string, SDL_Texture*> textures;
	std::unordered_map<std::string, TTF_Font*> fonts;
	std::unordered_map<std::string, Label*> labels;
};