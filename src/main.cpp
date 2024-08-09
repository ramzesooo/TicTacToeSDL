#include "app.h"
#include <memory>

int main(int argc, char** args)
{
	App app{ "TicTacToe", 800, 600 };

	while (app.IsGameRunning())
	{
		while (SDL_PollEvent(app.GetEvent()))
		{
			if (app.IsMenuRunning())
			{
				app.Menu();
			}
			else
			{
				app.EventHandler();
				app.Update();
				app.Render();
			}
		}
	}

	printf("Last catched error from SDL: %s\n", SDL_GetError());

	return 0;
}