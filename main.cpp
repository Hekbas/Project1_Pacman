#include "Game.h"

int main(int argc, char* args[])
{
	Game game;
	if (!game.Init()) return -1;

	bool end = false;

	while (game.UpdateMenu() == false)
	{
		if (game.GetContinueMenu() == true)
		{
			end = false;
			while (!end)
			{
				end = game.Update();

				game.Logic_Pacman();

				if (game.CheckForDeath() == false)
					game.Logic_Ghost();

				game.CheckForDeath();
				game.Frightened();

				game.Draw();
				SDL_Delay(100);
			}

			game.ResetVariables();
		}
	}

	game.Release();
	return 0;
}