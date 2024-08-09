#pragma once

class App;
class Manager;
struct Button;
struct Resolution;

class MainMenu
{
public:
	MainMenu(App* appPtr, SDL_Renderer* rendererPtr, Manager* managerPtr);
	~MainMenu();

	void EventHandler(SDL_Event* event);
	void Render();

	void PrimaryRender();
	void SettingsRender();

	Button* AddButton(const char* text, uint32_t x, uint32_t y, const char* labelID);
	void ButtonHover();

	void OnLPM();

	enum MenuState : uint32_t
	{
		none = 0,
		primary = 1, // main menu
		settings = 2,
	};

	void ClearHoveredButton();
private:
	MenuState m_MenuState = MenuState::primary;
	SDL_Event* m_Event;
	SDL_Renderer* renderer = nullptr;
	Manager* manager = nullptr;
	App* app = nullptr;
	Button* hoveredButton = nullptr;

	uint32_t currentResolution = 1;

	std::vector<Button*> primaryButtons;
	std::vector<Button*> settingsButtons;
	std::unordered_map<uint32_t, Resolution> resolutions;
};