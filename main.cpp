#include "Game.h"

int main(int argc, char* args[])
{
    Game game;
    if (!game.Init()) return -1;

    bool end = false;
    bool gameover = false;
    bool gomenu = false;

    while (game.UpdateMenu() == false)
    {
        game.DrawMenu();

        if (game.GetContinueMenu() == true)
        {
            end = false;
            gameover = false;

            game.startIntro();

            while (!end)
            {
                gameover = game.Update();
                game.Logic_Pacman();

                if (game.CheckForDeath() == false)
                    game.Logic_Ghost();

                game.CheckForDeath();
                game.Frightened();

                game.Draw();
                SDL_Delay(100);

                if (gameover == true)
                {
                    while (!gomenu)
                        gomenu = game.GameOver();

                    end = true;
                }
            }
        }

        game.ResetVariables();
    }

    game.Release();
    return 0;
}