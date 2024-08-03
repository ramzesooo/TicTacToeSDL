#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <map>
#include <unordered_map>
#include <string>
#include "stdlib.h"
#include "time.h"
#include "additional.h"

class Manager;
struct Board;

class App
{
public:
	App(const char* title, int width, int height);
	~App();

	bool Init();
	void Clean();

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

	void HandleKeyUp();
	void HandleKeyDown();
private:
	SDL_Event event;
public:
	// associated to rendering
	void DrawBoard();

	// associated to updating
	void CheckWinner();
	void Finish();
private:
	std::unordered_map<uint16_t, Board*> theBoard;
	std::vector<uint16_t> boardX, boardY;
	Manager* manager = nullptr;
	contents winner = square; // will return null
};

struct Board
{
	SDL_Rect src{ 0, 0, 32, 32 };
	SDL_Rect dest{ 0, 0, 32, 32 };
	uint8_t idX = 0, idY = 0; // posX & posY on the board (for example x == 0 && y == 0 means it's the first square)
	contents content = square;
};