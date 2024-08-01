#include "manager.h"
#include <iostream>

App::App(const char* title, int width, int height)
{
	std::cout << "Loading game..." << std::endl;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout << "Failed to load a game: " << SDL_GetError() << "\n";
		return;
	}

	window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
	if (!window)
	{
		std::cout << "Failed to create a window: " << SDL_GetError() << "\n";
		return;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer)
	{
		std::cout << "Failed to create a renderer: " << SDL_GetError() << "\n";
		return;
	}

	// returns 0 on success and -1 on failure
	if (TTF_Init() == -1)
	{
		std::cout << "Failed to Init TTF: " << TTF_GetError() << "\n";
		return;
	}

	manager = new Manager(renderer);

	SDL_RenderSetVSync(renderer, 1);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	bIsRunning = true;

	Init();
}

App::~App()
{
	std::cout << "Destroyed the game\n";
}

void App::Init()
{
	manager->LoadTexture("square", "assets/square_32x32.png");
	manager->LoadTexture("circle", "assets/circle.png");
	manager->LoadTexture("cross", "assets/cross.png");

	manager->Log();

	uint32_t boardSizeX = 3;
	uint32_t boardSizeY = 3;

	uint32_t size = 32;

	uint32_t startX = (400 - (size / 2)) - (size * (boardSizeX - 3)); // TODO: fix this terrible math
	uint32_t startY = (300 - (size / 2)) - (size * (boardSizeY - 3)); // TODO: fix this terrible math

	uint32_t posX = startX;
	uint32_t posY = startY;

	for (uint32_t y = 0; y < boardSizeY; ++y)
	{
		for (uint32_t x = 0; x < boardSizeX; ++x)
		{
			Board board;
			board.idX = x;
			board.idY = y;
			board.dest.x = posX + (size * x);
			board.dest.y = posY + (size * y);
			board.dest.w = board.dest.h = size;
			board.content = "square";
			theBoard.push_back(board);
		}
	}

	std::cout << "The game has been loaded\n";
}

SDL_Renderer* App::GetRenderer() const
{
	return renderer;
}

bool App::GetRunningState() const
{
	return bIsRunning;
}

void App::EventHandler()
{
	SDL_PollEvent(&event);

	switch (event.type)
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
	default:
		break;
	}
}

void App::Update()
{

}

void App::Render()
{
	SDL_RenderClear(renderer);
	DrawBoard();
	SDL_RenderPresent(renderer);
}

void App::Clean()
{
	if (manager)
	{
		delete manager;
		manager = nullptr;
	}
	
	bIsRunning = false;
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	std::cout << "Cleaned the game\n";
}

void App::HandleKeyUp()
{
	/*switch (event.key.keysym.sym)
	{
	case 0:
		break;
	default:
		break;
	}*/
}

void App::HandleKeyDown()
{
	switch (event.key.keysym.sym)
	{
	case SDLK_ESCAPE:
		this->Clean();
		break;
	default:
		break;
	}
}

void App::DrawBoard()
{
	for (auto it = theBoard.begin(); it != theBoard.end(); ++it)
	{
		printf("idX: %d, idY: %d, ", it->idX, it->idY);
		manager->Draw(it->content.c_str(), it->dest);
	}
}