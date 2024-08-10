#include "manager.h"
#include "menu.h"
#include <iostream>

App::App(const char* title, int width, int height)
{
	SDL_PollEvent(&m_event);

	std::cout << "Loading game..." << std::endl;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout << "Failed to init SDL: " << SDL_GetError() << "\n";
		return;
	}

	window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
	if (!window)
	{
		std::cout << "Failed to create a window: " << SDL_GetError() << "\n";
		return;
	}

	SDL_Surface* iconSurface = IMG_Load("assets/icon.png");
	SDL_SetWindowIcon(window, iconSurface);
	SDL_FreeSurface(iconSurface);

	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer)
	{
		std::cout << "Failed to create a renderer: " << SDL_GetError() << "\n";
		return;
	}

	// returns 0 on success and -1 on failure
	if (TTF_Init() == -1)
	{
		std::cout << "Failed to init TTF: " << TTF_GetError() << "\n";
		return;
	}

	SDL_RenderSetVSync(renderer, 1);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	bIsRunning = Init();
}

App::~App()
{
	Clean();
	std::cout << "Destroyed the game\n";
}

bool App::Init()
{
	manager = new Manager(renderer, this);

	manager->LoadTexture((uint32_t)contents::square, "assets/square_32x32.png");
	manager->LoadTexture((uint32_t)contents::circle, "assets/circle.png");
	manager->LoadTexture((uint32_t)contents::cross, "assets/cross.png");
	manager->LoadTexture("line", "assets/line.png");

	manager->LoadFont("default", "assets/F25_Bank_Printer.ttf", 16);
	manager->LoadFont("defaultXL", "assets/F25_Bank_Printer.ttf", 24);
	manager->LoadFont("Rostack", "assets/rostack.otf", 36);

	manager->CreateLabel("default", "text", 300, 130, &white, "turnInfo");;

	uint32_t size = 32; // size to display

	uint32_t startX = ((WINDOW_WIDTH / 2) - (size / 2)) - size; // TODO: fix this terrible math
	uint32_t startY = ((WINDOW_HEIGHT / 2) - (size / 2)) - size; // TODO: fix this terrible math

	uint32_t posX = startX;
	uint32_t posY = startY;

	uint16_t UUID = 0;

	for (uint32_t y = 0; y < 3; ++y)
	{
		for (uint32_t x = 0; x < 3; ++x)
		{
			boardTiles[UUID] = new BoardTile();

			boardTiles[UUID]->dest.x = posX + (size * x);
			boardTiles[UUID]->dest.y = posY + (size * y);
			boardTiles[UUID]->dest.w = boardTiles[UUID]->dest.h = size;

			boardTiles[UUID]->content = contents::square;
			boardTiles[UUID]->UUID = UUID;
			UUID++;
		}
	}

	std::cout << "The game has been loaded\n";

	menu = new MainMenu(this, renderer, manager);

	manager->Log();

	return true;
}

SDL_Renderer* App::GetRenderer() const
{
	return renderer;
}

bool App::IsGameRunning() const
{
	return bIsRunning;
}

void App::SetGameState(bool newState)
{
	bIsRunning = newState;
}

bool App::IsMenuRunning() const
{
	return bIsMenuShowing;
}

void App::SetMenuState(bool newState)
{
	bIsMenuShowing = newState;
}

void App::Menu()
{
	menu->EventHandler(&m_event);
	menu->Render();
}

SDL_Event* App::GetEvent()
{
	return &m_event;
}

void App::EventHandler()
{
	switch (m_event.type)
	{
	case SDL_QUIT:
		bIsRunning = false;
		break;
	case SDL_KEYUP:
		HandleKeyUp();
		break;
	case SDL_KEYDOWN:
		HandleKeyDown();
		break;
	case SDL_MOUSEBUTTONDOWN:
		OnLPM(m_event.button.x, m_event.button.y);
		break;
	default:
		break;
	}
}

void App::Update()
{
	// 0 == square == false
	// 1 == circle
	// 2 == cross
	if ((uint32_t)winner || CheckWinner())
	{
		return;
	}

	std::string turnInfo = "";

	if (nextTurn == contents::circle)
	{
		turnInfo = "Now it's O's turn!";
	}
	else
	{
		turnInfo = "Now it's X's turn!";
	}

	manager->UpdateLabelText(turnInfo, "turnInfo", &white);
	manager->CenterLabelX("turnInfo");
}

void App::Render()
{
	SDL_RenderClear(renderer);

	DrawBoard();
	DrawWinnerLine();

	manager->DrawLabel("turnInfo");

	SDL_RenderPresent(renderer);
}

void App::Clean()
{
	for (auto& tile : boardTiles)
	{
		delete tile.second;
		tile.second = nullptr;
	}

	if (menu)
	{
		delete menu;
		menu = nullptr;
	}

	if (manager)
	{
		delete manager;
		manager = nullptr;
	}
	
	bIsRunning = false;
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	renderer = nullptr;
	window = nullptr;

	std::cout << "Cleaned the game\n";
}

void App::HandleKeyUp()
{
	/*switch (m_event.key.keysym.sym)
	{
	case 0:
		break;
	default:
		break;
	}*/
}

void App::HandleKeyDown()
{
	switch (m_event.key.keysym.sym)
	{
	case SDLK_ESCAPE:
		bIsRunning = false;
		break;
	default:
		break;
	}
}

void App::OnLPM(int x, int y)
{
	if ((uint32_t)winner)
	{
		return;
	}

	for (const auto& tile : boardTiles)
	{
		if (x > tile.second->dest.x + tile.second->dest.w || x < tile.second->dest.x
			|| y > tile.second->dest.y + tile.second->dest.h || y < tile.second->dest.y)
		{
			continue;
		}

		if (tile.second->content != contents::square)
		{
			return;
		}

		tile.second->content = nextTurn;
		nextTurn = static_cast<contents>((uint32_t)nextTurn ^ 3); // 1 ^ 3 = 2, 2 ^ 3 = 1
		break;
	}
}

bool App::CheckWinner() // needs to be optimized
{
	// check for existing all places and is the board filled already
	bool bAbleToMove = false;

	for (const auto& tile : boardTiles)
	{
		if (!tile.second)
		{
			std::cout << "Missing a place in the board!" << std::endl;
			bIsRunning = false;
			return true;
		}

		if (tile.second->content == contents::square)
		{
			bAbleToMove = true;
			break;
		}
	}

	// board:
	// 0, 1, 2
	// 3, 4, 5
	// 6, 7, 8

	// check horizontal and vertical possibilites
	for (uint16_t i = 0; i < 3; ++i)
	{
		uint16_t UUID = 3 * i; // horizontal
		if (boardTiles[UUID]->content != contents::square)
		{
			if (boardTiles[UUID]->content == boardTiles[1 + UUID]->content && boardTiles[1 + UUID]->content == boardTiles[2 + UUID]->content)
			{
				winner = boardTiles[UUID]->content;
				lineAngle = 90;
				lineDest.x = boardTiles[UUID]->dest.x + lineDest.w;
				lineDest.y = boardTiles[UUID]->dest.y - lineDest.w;
				Finish();
				return true;
			}
		}

		UUID = i; // vertical
		if (boardTiles[UUID]->content != contents::square)
		{
			if (boardTiles[UUID]->content == boardTiles[3 + UUID]->content && boardTiles[3 + UUID]->content == boardTiles[6 + UUID]->content)
			{
				winner = boardTiles[UUID]->content;
				lineAngle = 0;
				lineDest.x = boardTiles[UUID]->dest.x;
				lineDest.y = boardTiles[UUID]->dest.y;
				Finish();
				return true;
			}
		}
	}

	for (uint16_t i = 0; i < 2; ++i) // diagonally
	{
		// 0, 4, 8
		// 2, 4, 6
		if (boardTiles[4]->content == contents::square)
		{
			break;
		}

		// i = 0: 0, 4, 8
		// i = 1: 2, 4, 6
		BoardTile* b1 = boardTiles[2 * i];
		BoardTile* b2 = boardTiles[4];
		BoardTile* b3 = boardTiles[8 - 2 * i];

		if (b1->content == b2->content && b2->content == b3->content)
		{
			winner = boardTiles[4]->content;

			lineDest.x = b1->dest.x + lineDest.w * (1 - 2 * i);
			lineDest.y = b1->dest.y - (lineDest.w / 2);
			lineAngle = 45 * (3 - 2 * i);
			lineDest.h = 32 * 4; // * 4 because of the angle :)

			Finish();
			return true;
		}
	}

	if (!bAbleToMove)
	{
		winner = contents::tie;
		Finish();
		return true;
	}

	return false;
}

void App::Finish()
{
	std::string newLabelText = "";

	switch (winner)
	{
	case contents::square:
		std::cout << "Something's wrong!!" << std::endl;
		Clean();
		break;
	case contents::circle:
		std::cout << "Circle is the winner!" << std::endl;
		newLabelText = "O is the winner!";
		break;
	case contents::cross:
		std::cout << "Cross is the winner!" << std::endl;
		newLabelText = "X is the winner!";
		break;
	case contents::tie:
		std::cout << "Nobody won!" << std::endl;
		newLabelText = "TIE!";
		break;
	default:
		break;
	}

	manager->UpdateLabelText(newLabelText, "turnInfo", &green);
	manager->CenterLabelX("turnInfo");
}

void App::DrawBoard()
{
	for (const auto& tile : boardTiles)
	{
		if (!tile.second)
		{
			std::cout << "Can't draw the board if there is missing any..." << std::endl;
			break;
		}
		manager->DrawTexture((uint32_t)tile.second->content, &this->src, &tile.second->dest);
	}
}

void App::DrawWinnerLine()
{
	if ((uint32_t)winner)
	{
		manager->DrawTexture("line", &this->src, &lineDest, lineAngle);
	}
}

void App::AdjustToResolution()
{
	uint32_t size = 32; // size to display

	uint32_t startX = ((WINDOW_WIDTH / 2) - (size / 2)) - size; // TODO: fix this terrible math
	uint32_t startY = ((WINDOW_HEIGHT / 2) - (size / 2)) - size; // TODO: fix this terrible math

	uint32_t posX = startX;
	uint32_t posY = startY;

	uint16_t UUID = 0;

	for (uint32_t y = 0; y < 3; ++y)
	{
		for (uint32_t x = 0; x < 3; ++x)
		{
			boardTiles[UUID]->dest.x = posX + (size * x);
			boardTiles[UUID]->dest.y = posY + (size * y);
			boardTiles[UUID]->dest.w = boardTiles[UUID]->dest.h = size;

			boardTiles[UUID]->UUID = UUID;
			UUID++;
		}
	}
}