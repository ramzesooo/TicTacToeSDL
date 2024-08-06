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
	SDL_Event* GetEvent();

	void EventHandler();
	void Update();
	void Render();

	void HandleKeyUp();
	void HandleKeyDown();
private:
	SDL_Event m_event;
public:
	// EventHandler:
	void OnLPM(int x, int y);

	// Update:
	bool CheckWinner(); // returns true if gotta don't execute Update() or got a winner
	void Finish();

	// Render:
	void DrawBoard();
	void DrawWinnerLine();
private:
	SDL_Color white{ 255, 255, 255, 255 };
	SDL_Color green{ 30, 255, 30, 255 };
	contents nextTurn = circle;
	std::unordered_map<uint16_t, Board*> theBoard;
	Manager* manager = nullptr;
	contents winner = square; // returns false
	SDL_Rect src{ 0, 0, 32, 32 }; // used for Board struct, it's the same every time, so doesn't need to put it into the struct
	SDL_Rect lineDest{ 0, 0, 32, 96 }; // for winning line
	double lineAngle = 0;
};

struct Board
{
	SDL_Rect dest{ 0, 0, 32, 32 };
	contents content = square;
	uint16_t UUID = 0;
};