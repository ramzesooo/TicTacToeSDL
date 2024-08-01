#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <map>
#include <unordered_map>
#include <string>

class Manager;

struct Board
{
	SDL_Rect dest = { 0, 0, 32, 32 };
	uint8_t idX = 0, idY = 0; // posX & posY on the board (for example x == 0 && y == 0 means it's the first square)
	std::string content = "square"; // texture id or empty string
};

class App
{
public:
	App(const char* title, int width, int height);
	~App();

	void Init();

	SDL_Renderer* GetRenderer() const;
	bool GetRunningState() const;
private:
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	bool bIsRunning = false;
public:
	void EventHandler();
	void Update();
	void Render();
	void Clean();

	void HandleKeyUp();
	void HandleKeyDown();
private:
	SDL_Event event;
public:
	void DrawBoard();
private:
	std::vector<Board> theBoard;
	Manager* manager = nullptr;
};