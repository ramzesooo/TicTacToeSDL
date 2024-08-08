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

MainMenu::MainMenu(App* appPtr, SDL_Renderer* rendererPtr, Manager* managerPtr) : app(appPtr), renderer(rendererPtr), manager(managerPtr)
{
	manager->CreateLabel("default", "Tic Tac Toe", 0, 100, &app->white, "gameName");
	manager->CenterLabelX("gameName");

	AddButton("Play", 370, 380, "Play");
	AddButton("Settings", 370, 420, "Settings");
	AddButton("Quit", 370, 460, "Quit");

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
	case none:
		break;
	case primary:
		PrimaryRender();
		break;
	case settings:
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

void MainMenu::AddButton(const char* text, uint32_t x, uint32_t y, const char* labelID)
{
	Button* button = new Button();

	manager->CreateLabel("default", text, x, y, &app->white, labelID);
	manager->CenterLabelX(labelID);

	button->labelID = labelID;
	button->originalPos = manager->GetLabelRect(labelID);
	primaryButtons.push_back(button);
}

void MainMenu::ButtonHover()
{
	int mouseX = m_Event->button.x;
	int mouseY = m_Event->button.y;

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
				continue;
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
		manager->UpdateFont(hoveredButton->labelID, "default");
		manager->CenterLabelX(hoveredButton->labelID);
		hoveredButton = nullptr;
	}
}

void MainMenu::OnLPM()
{
	switch (m_MenuState)
	{
	case none:
		app->SetMenuState(false);
		break;
	case primary:
		if (!hoveredButton)
		{
			return;
		}

		if (hoveredButton->labelID == "Play")
		{
			m_MenuState = none;
			app->SetMenuState(false);
		}
		else if (hoveredButton->labelID == "Settings")
		{

		}
		else if (hoveredButton->labelID == "Quit")
		{
			app->SetGameState(false);
		}
		else
		{
			std::cout << "Couldn't recognize pressed button" << std::endl;
		}
		break;
	case settings:
		break;
	default:
		break;
	}
}