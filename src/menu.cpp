#include "app.h"
#include "manager.h"
#include "menu.h"
#include <iostream>

struct Button
{
	SDL_Rect* originalPos;
	SDL_Rect* hoveredPos;
	std::string labelID;
};

struct Resolution
{
	int x = 0;
	int y = 0;
	std::string tag = "";
};

MainMenu::MainMenu(App* appPtr, SDL_Renderer* rendererPtr, Manager* managerPtr) : app(appPtr), renderer(rendererPtr), manager(managerPtr)
{
	Resolution resolution{ 640, 480, "640x480" };
	resolutions[0] = resolution;

	resolution.x = 800;
	resolution.y = 600;
	resolution.tag = "800x600";
	resolutions[1] = resolution;

	resolution.x = 1024;
	resolution.y = 768;
	resolution.tag = "1024x768";
	resolutions[2] = resolution;

	resolution.x = 1280;
	resolution.y = 720;
	resolution.tag = "1280x720";
	resolutions[3] = resolution;

	for (const auto& res : resolutions)
	{
		printf("Loaded resolution #%d: (x: %d, y: %d, tag: %s)\n", res.first, res.second.x, res.second.y, res.second.tag.c_str());
	}

	manager->CreateLabel("default", "Tic Tac Toe", 0, 100, &app->white, "gameName");
	manager->CenterLabelX("gameName");

	// primaryButtons
	primaryButtons.push_back(AddButton("Play", 370, 380, "Play"));
	primaryButtons.push_back(AddButton("Settings", 370, 420, "Settings"));
	primaryButtons.push_back(AddButton("Quit", 370, 460, "Quit"));

	// settingsButtons
	settingsButtons.push_back(AddButton("Resolution: 800x600", 370, 380, "ResolutionSettings"));
	settingsButtons.push_back(AddButton("Save", 370, 420, "SaveSettings"));
	settingsButtons.push_back(AddButton("Back", 370, 460, "BackSettings"));

	SDL_SetRenderDrawColor(renderer, 10, 10, 10, 255);

	std::cout << "Menu created" << std::endl;
}

MainMenu::~MainMenu()
{
	for (auto& button : primaryButtons)
	{
		delete button;
		button = nullptr;
	}

	for (auto& button : settingsButtons)
	{
		delete button;
		button = nullptr;
	}

	std::cout << "Menu destroyed" << std::endl;
}

void MainMenu::EventHandler(SDL_Event* event)
{
	m_Event = event;
	ButtonHover();

	switch (m_Event->type)
	{
	case SDL_QUIT:
		app->SetGameState(false);
		break;
	case SDL_MOUSEBUTTONDOWN:
		OnLPM();
		break;
	default:
		break;
	}
}

void MainMenu::Render()
{
	SDL_RenderClear(renderer);

	manager->DrawLabel("gameName");
	
	switch (m_MenuState)
	{
	case MenuState::none:
		app->SetMenuState(false);
		break;
	case MenuState::primary:
		PrimaryRender();
		break;
	case MenuState::settings:
		manager->UpdateLabelText("Resolution: " + resolutions[currentResolution].tag, "ResolutionSettings", &app->white);
		SettingsRender();
		break;
	default:
		break;
	}

	SDL_RenderPresent(renderer);
}

void MainMenu::PrimaryRender()
{
	for (const auto& button : primaryButtons)
	{
		manager->DrawLabel(button->labelID);
	}
}

void MainMenu::SettingsRender()
{
	for (const auto& button : settingsButtons)
	{
		manager->DrawLabel(button->labelID);
	}
}

Button* MainMenu::AddButton(const char* text, uint32_t x, uint32_t y, const char* labelID)
{
	Button* button = new Button();

	manager->CreateLabel("default", text, x, y, &app->white, labelID);
	manager->CenterLabelX(labelID);

	button->labelID = labelID;
	button->originalPos = manager->GetLabelRect(labelID);
	//primaryButtons.push_back(button);
	return button;
}

void MainMenu::ButtonHover()
{
	int mouseX = m_Event->button.x;
	int mouseY = m_Event->button.y;

	if (m_MenuState == MenuState::primary)
	{
		for (auto& button : primaryButtons)
		{
			if (hoveredButton && hoveredButton == button)
			{
				if (mouseX >= button->hoveredPos->x && mouseX <= button->hoveredPos->w + button->hoveredPos->x
					&& mouseY >= button->hoveredPos->y && mouseY <= button->hoveredPos->h + button->hoveredPos->y)
				{
					return; // TODO: FIX THIS SHIT CODE
				}
				else
				{
					break;
				}
			}
			else
			{
				if (mouseX >= button->originalPos->x && mouseX <= button->originalPos->w + button->originalPos->x
					&& mouseY >= button->originalPos->y && mouseY <= button->originalPos->h + button->originalPos->y)
				{
					hoveredButton = button;
					manager->UpdateFont(button->labelID, "defaultXL");
					manager->CenterLabelX(button->labelID);
					button->hoveredPos = manager->GetLabelRect(button->labelID);
					return;
				}
			}
		}

		if (hoveredButton)
		{
			ClearHoveredButton();
		}
	}
	else if (m_MenuState == MenuState::settings)
	{
		for (auto& button : settingsButtons)
		{
			if (hoveredButton && hoveredButton == button)
			{
				if (mouseX >= button->hoveredPos->x && mouseX <= button->hoveredPos->w + button->hoveredPos->x
					&& mouseY >= button->hoveredPos->y && mouseY <= button->hoveredPos->h + button->hoveredPos->y)
				{
					return; // TODO: FIX THIS SHIT CODE
				}
				else
				{
					break;
				}
			}
			else
			{
				if (mouseX >= button->originalPos->x && mouseX <= button->originalPos->w + button->originalPos->x
					&& mouseY >= button->originalPos->y && mouseY <= button->originalPos->h + button->originalPos->y)
				{
					hoveredButton = button;
					manager->UpdateFont(button->labelID, "defaultXL");
					manager->CenterLabelX(button->labelID);
					button->hoveredPos = manager->GetLabelRect(button->labelID);
					return;
				}
			}
		}

		if (hoveredButton)
		{
			ClearHoveredButton();
		}
	}
}

void MainMenu::OnLPM()
{
	switch (m_MenuState)
	{
	case MenuState::none:
		app->SetMenuState(false);
		break;
	case MenuState::primary:
		if (!hoveredButton)
		{
			return;
		}

		if (hoveredButton->labelID == "Play")
		{
			m_MenuState = MenuState::none;
			app->SetMenuState(false);
		}
		else if (hoveredButton->labelID == "Settings")
		{
			m_MenuState = MenuState::settings;
		}
		else if (hoveredButton->labelID == "Quit")
		{
			app->SetGameState(false);
		}
		else
		{
			std::cout << "Couldn't recognize pressed button" << std::endl;
		}
		
		ClearHoveredButton();
		break;
	case MenuState::settings:
		if (!hoveredButton)
		{
			return;
		}

		if (hoveredButton->labelID == "ResolutionSettings")
		{
			currentResolution++;
			if (currentResolution >= resolutions.size())
			{
				currentResolution = 0;
			}

			std::string newText = "Resolution: " + resolutions[currentResolution].tag;
			manager->UpdateLabelText(newText, hoveredButton->labelID, &app->white);
		}
		else if (hoveredButton->labelID == "SaveSettings")
		{
			// if the resolution stays the same, then don't continue
			if (app->WINDOW_WIDTH == resolutions[currentResolution].x && app->WINDOW_HEIGHT == resolutions[currentResolution].y)
			{
				return;
			}

			app->WINDOW_WIDTH = resolutions[currentResolution].x;
			app->WINDOW_HEIGHT = resolutions[currentResolution].y;

			SDL_Window* window = SDL_RenderGetWindow(renderer);
			SDL_SetWindowSize(window, app->WINDOW_WIDTH, app->WINDOW_HEIGHT);
			SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

			manager->CenterLabelX("gameName");

			for (const auto& button : primaryButtons)
			{
				manager->CenterLabelX(button->labelID);
			}

			for (const auto& button : settingsButtons)
			{
				manager->CenterLabelX(button->labelID);
			}

			app->AdjustToResolution();
		}
		else if (hoveredButton->labelID == "BackSettings")
		{
			m_MenuState = MenuState::primary;

			ClearHoveredButton();
		}
		else
		{
			std::cout << "Couldn't recognize pressed button" << std::endl;
		}

		break;
	default:
		break;
	}
}

void MainMenu::ClearHoveredButton()
{
	if (!hoveredButton)
	{
		return;
	}

	manager->UpdateFont(hoveredButton->labelID, "default");
	manager->CenterLabelX(hoveredButton->labelID);
	hoveredButton = nullptr;
}