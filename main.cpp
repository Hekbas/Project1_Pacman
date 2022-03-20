#include "Game.h"

int main(int argc, char* args[])
{
	Game game;
	if (!game.Init()) return -1;

	bool end = false;
	bool gameover = false;
	bool gomenu = false;

	while (game.Updat() == false)
	{
		if (game.GetContinueMenu() == true)
		{
        game.DrawMenu();

        if (game.GetContinueMenu() == true)
        {
          end = false;
          gameover = false;
          while (!end)
          {
            gameover = game.Update();
            game.Logic_Pacman();

            if (game.CheckForDeath() == false)
              game.Logic_Ghost();
            
            game.Draw();
            if (gameover == true) {
              while (!gomenu) {
                gomenu = game.GameOver();
              }
              end = true;
            }
            SDL_Delay(100);
          }
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