#pragma once
#include "app.h"

struct Label;

class Manager
{
public:
	Manager(SDL_Renderer* renderer);
	~Manager();

	void LoadTexture(const char* textureID, const char* path);
	void LoadTexture(contents textureID, const char* path);
	void DrawTexture(const char* textureID, SDL_Rect* src, SDL_Rect* dest, double angle = 0);
	void DrawTexture(contents textureID, SDL_Rect* src, SDL_Rect* dest, double angle = 0);

	void LoadFont(std::string fontID, const char* path, uint16_t fontSize);
	void CreateLabel(std::string fontID, std::string text, uint32_t x, uint32_t y, SDL_Color* color, std::string labelID);
	void UpdateLabelText(std::string text, std::string labelID, SDL_Color* color);
	void UpdateLabelPos(std::string labelID, uint32_t x, uint32_t y);
	SDL_Rect* GetLabelRect(std::string labelID);
	void DrawLabel(std::string labelID);

	void Log();
private:
	SDL_Renderer* renderer;
	std::unordered_map<std::string, SDL_Texture*> textures;
	std::unordered_map<std::string, TTF_Font*> fonts;
	std::unordered_map<std::string, Label*> labels;
};