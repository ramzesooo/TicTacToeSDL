#include "app.h"
#include <memory>

int main(int argc, char** args)
{
	std::unique_ptr<App> app = std::make_unique<App>("TicTacToe", 800, 600);

	while (app->GetRunningState())
	{
		app->EventHandler();
		app->Update();
		app->Render();
	}

	return 0;
}