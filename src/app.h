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
class MainMenu;
struct BoardTile;

class App
{
public:
	App(const char* title, int width, int height);
	~App();

	bool Init();
	void Clean();

	SDL_Renderer* GetRenderer() const;
	bool IsGameRunning() const;
	void SetGameState(bool newState);
	bool IsMenuRunning() const;
	void SetMenuState(bool newState);

	int WINDOW_WIDTH = 800;
	int WINDOW_HEIGHT = 600;
private:
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	bool bIsRunning = false;
	bool bIsMenuShowing = true;
// main menu begin
public:
	void Menu(); // this method triggers all needed for menu's functionality
private:
	MainMenu* menu = nullptr; // pointer to MainMenu class
// main menu end
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

	SDL_Color white{ 255, 255, 255, 255 };
	SDL_Color green{ 30, 255, 30, 255 };
private:
	contents nextTurn = contents::circle;
	std::unordered_map<uint16_t, BoardTile*> boardTiles;
	Manager* manager = nullptr;
	contents winner = contents::square; // NULL by default (contents::square)
	SDL_Rect src{ 0, 0, 32, 32 }; // used for BoardTile struct, it's the same every time, so doesn't need to put it into the struct
	SDL_Rect lineDest{ 0, 0, 32, 96 }; // for drawing the line of win
	double lineAngle = 0;
};

struct BoardTile
{
	SDL_Rect dest{ 0, 0, 32, 32 };
	contents content = contents::square;
	uint16_t UUID = 0;
};