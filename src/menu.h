#pragma once

class App;
class Manager;
struct Button;

class MainMenu
{
public:
	MainMenu(App* appPtr, SDL_Renderer* rendererPtr, Manager* managerPtr);
	~MainMenu();

	void EventHandler(SDL_Event* event);
	void Render();

	void PrimaryRender();

	void AddButton(const char* text, uint32_t x, uint32_t y, const char* labelID);
	void ButtonHover();

	void OnLPM();

	enum MenuState : uint32_t
	{
		none = 0,
		primary = 1, // main menu
		settings = 2,
	};
private:
	MenuState m_MenuState = primary;
	SDL_Event* m_Event;
	SDL_Renderer* renderer = nullptr;
	Manager* manager = nullptr;
	App* app = nullptr;
	Button* hoveredButton = nullptr;
	std::vector<Button*> primaryButtons;
};