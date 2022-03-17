#include "Game.h"

int main(int argc, char* args[])
{
	Game game;
	if (!game.Init()) return -1;

	bool end = false;
	while (!end)
	{
		end = game.Update();
		game.Logic_Pacman();
		game.Logic_Ghost();
		game.Draw();
		//SDL_Delay(200);
	}

	game.Release();
	return 0;
}