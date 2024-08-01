#pragma once
#include "app.h"

class Manager
{
public:
	Manager(SDL_Renderer* renderer);
	~Manager();

	void LoadTexture(const char* textureID, const char* path);
	void Draw(const char* textureID, SDL_Rect dest);

	void Log();
private:
	SDL_Renderer* renderer;
	std::unordered_map<std::string, SDL_Texture*> textures;
};