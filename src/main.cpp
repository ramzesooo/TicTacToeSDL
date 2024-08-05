#include "app.h"
#include <memory>

namespace TicTacToe
{
	static void Run()
	{
		srand(static_cast<uint32_t>(time(NULL)));

		std::unique_ptr<App> app = std::make_unique<App>("TicTacToe", 800, 600);

		while (app->GetRunningState())
		{
			while (SDL_PollEvent(app->GetEvent()))
			{
				app->EventHandler();
				app->Update();
				app->Render();
			}
		}

		printf("Last catched error from SDL: %s\n", SDL_GetError());
	}
}

int main(int argc, char** args)
{
	TicTacToe::Run();

	return 0;
}