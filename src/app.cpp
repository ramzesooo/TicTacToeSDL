#include "manager.h"
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
	manager = new Manager(renderer);

	manager->LoadTexture(square, "assets/square_32x32.png");
	manager->LoadTexture(circle, "assets/circle.png");
	manager->LoadTexture(cross, "assets/cross.png");
	manager->LoadTexture("line", "assets/line.png");

	manager->Log();

	uint32_t size = 32; // size to display

	uint32_t startX = (400 - (size / 2)) - size; // TODO: fix this terrible math
	uint32_t startY = (300 - (size / 2)) - size; // TODO: fix this terrible math

	uint32_t posX = startX;
	uint32_t posY = startY;

	uint16_t UUID = 0;

	for (uint32_t y = 0; y < 3; ++y)
	{
		for (uint32_t x = 0; x < 3; ++x)
		{
			theBoard[UUID] = new Board();

			theBoard[UUID]->dest.x = posX + (size * x);
			theBoard[UUID]->dest.y = posY + (size * y);
			theBoard[UUID]->dest.w = theBoard[UUID]->dest.h = size;

			theBoard[UUID]->content = square;
			theBoard[UUID]->UUID = UUID;
			UUID++;
		}
	}

	std::cout << "The game has been loaded\n";
	return true;
}

SDL_Renderer* App::GetRenderer() const
{
	return renderer;
}

bool App::GetRunningState() const
{
	return bIsRunning;
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
	if (winner || CheckWinner())
	{
		return;
	}

	
}

void App::Render()
{
	SDL_RenderClear(renderer);

	if (winner)
	{
		DrawWinnerLine();
	}
	DrawBoard();

	SDL_RenderPresent(renderer);
}

void App::Clean()
{
	for (auto& board : theBoard)
	{
		delete board.second;
		board.second = nullptr;
	}

	if (manager)
	{
		delete manager;
		manager = nullptr;
	}
	
	bIsRunning = false;
	SDL_DestroyRenderer(renderer);
	renderer = nullptr;
	SDL_DestroyWindow(window);
	window = nullptr;
	SDL_Quit();
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

void App::OnLPM(uint32_t x, uint32_t y)
{
	for (const auto& board : theBoard)
	{
		if (x < board.second->dest.x + board.second->dest.w && x >= board.second->dest.x
			&& y >= board.second->dest.y && y < board.second->dest.y + board.second->dest.h)
		{
			printf("#%d: (%d, %d)\n", board.first, board.second->dest.x, board.second->dest.y);
		}
	}
}

bool App::CheckWinner() // need to be optimized
{
	// check for existing all places and is the board filled already
	bool bAbleToMove = false;

	for (const auto& board : theBoard)
	{
		if (!board.second)
		{
			std::cout << "Missing a place in the board!" << std::endl;
			Clean();
			return true;
		}
		else
		{
			if (board.second->content == square)
			{
				bAbleToMove = true;
				break;
			}
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
		if (!theBoard[UUID]->content == square)
		{
			if (theBoard[UUID]->content == theBoard[1 + UUID]->content && theBoard[1 + UUID]->content == theBoard[2 + UUID]->content)
			{
				winner = theBoard[UUID]->content;
				lineAngle = 90;
				lineDest.x = theBoard[UUID]->dest.x + lineDest.w;
				lineDest.y = theBoard[UUID]->dest.y - lineDest.w;
				Finish();
				return true;
			}
		}

		UUID = i; // vertical
		if (!theBoard[UUID]->content == square)
		{
			if (theBoard[UUID]->content == theBoard[3 + UUID]->content && theBoard[3 + UUID]->content == theBoard[6 + UUID]->content)
			{
				winner = theBoard[UUID]->content;
				lineAngle = 0;
				lineDest.x = theBoard[UUID]->dest.x;
				lineDest.y = theBoard[UUID]->dest.y;
				Finish();
				return true;
			}
		}
	}

	for (uint16_t i = 0; i < 2; ++i) // diagonally
	{
		// 0, 4, 8
		// 2, 4, 6
		if (theBoard[4]->content == square)
		{
			break;
		}

		// i = 0: 0, 4, 8
		// i = 1: 2, 4, 6
		Board* b1 = theBoard[2 * i];
		Board* b2 = theBoard[4];
		Board* b3 = theBoard[8 - 2 * i];

		if (b1->content == b2->content && b2->content == b3->content)
		{
			winner = theBoard[4]->content;

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
		winner = tie;
		Finish();
		return true;
	}

	return false;
}

void App::Finish()
{
	switch (winner)
	{
	case square:
		std::cout << "Something's wrong!!" << std::endl;
		Clean();
		break;
	case circle:
		std::cout << "Circle is the winner!" << std::endl;
		break;
	case cross:
		std::cout << "Cross is the winner!" << std::endl;
		break;
	case tie:
		std::cout << "Nobody won!" << std::endl;
		break;
	default:
		break;
	}
}

void App::DrawBoard()
{
	for (const auto& board : theBoard)
	{
		if (!board.second)
		{
			std::cout << "Can't draw the board if there is missing any..." << std::endl;
			break;
		}
		manager->Draw(board.second->content, &this->src, &board.second->dest);
	}
}

void App::DrawWinnerLine()
{
	manager->Draw("line", &this->src, &lineDest, lineAngle);
}