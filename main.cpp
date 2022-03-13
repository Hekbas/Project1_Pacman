#include "Game.h"

int main(int argc, char* args[])
{
	Game game;
	if (!game.Init()) return -1;

	bool end = false;
	while (!end)
	{
		// force a quit
		while (Mix_Init(0))
			Mix_Quit();

		end = game.Update();
		game.Draw();
	}

	game.Release();
	return 0;
}