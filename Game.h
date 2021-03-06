#pragma once
#include <windows.h>
#include<sstream>

#include "SDL/include/SDL.h"
#include "SDL_image/include/SDL_image.h"
#include "SDL_Mixer/include/SDL_mixer.h"

#pragma comment( lib, "SDL/libx86/SDL2.lib" )
#pragma comment( lib, "SDL/libx86/SDL2main.lib" )
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )
#pragma comment( lib, "SDL_Mixer/libx86/SDL2_mixer.lib" )

#include "Entity.h"

#define LOG(txt) OutputDebugString(txt)
#define WINDOW_WIDTH	896
#define WINDOW_HEIGHT	992
#define MAX_KEYS		256
#define H				31
#define W				29

class Game
{
public:
	Game();
	Game(char playfield[H][W]);
	~Game();

	bool Init();
	bool LoadImages();
	void Release();
	bool Input();
	bool LoadSound();

	void Frightened();
	bool CheckForDeath();
	void Logic_Pacman();
	void Logic_Ghost();

	void startIntro();
	bool Update();
	void GetRect2(int* x, int* y, int* w, int* h);
	void GetRect3(int* posx, int* posy, int* w, int* h, int x, int y, int _w, int _h);
	void Draw();
	void DrawMenu();

	bool UpdateMenu();
	bool GetContinueMenu();

	void ResetVariables();
	bool GameOver();

private:
	SDL_Window* Window;
	SDL_Renderer* Renderer;

	SDL_Texture* img_background, * img_food, * img_powerup,  * img_menu, * img_gameover, * img_life, * img_score;
	SDL_Texture* img_pacman, * pacman_up, * pacman_down, * pacman_left, * pacman_right, * pacman_birth;
	SDL_Texture* img_ghost, * img_ghostRed, * img_scaredBlue;

	Mix_Chunk* gamestartsound, * wakasound_1, * wakasound_2, * deathsound, * eatfruitsound, * eatghostsound, * powerpelletsound, *siren_1;

	Entity Pacman, GhostRed, Scene, Food[H][W], Status;

	bool continuemenu;
	bool god_mode;

	enum KEY_STATE { KEY_IDLE, KEY_DOWN, KEY_REPEAT, KEY_UP };
	KEY_STATE keys[MAX_KEYS];

	char playfieldOriginal[H][W] =
	{
		{"############################"},
		{"#????????????##????????????#"},
		{"#+####?#####?##?#####?####+#"},
		{"#?####?#####?##?#####?####?#"},
		{"#?####?#####?##?#####?####?#"},
		{"#??????????????????????????#"},
		{"#?####?##?########?##?####?#"},
		{"#?####?##?########?##?####?#"},
		{"#??????##????##????##??????#"},
		{"######?##### ## #####?######"},
		{"######?##### ## #####?######"},
		{"######?##          ##?######"},
		{"######?## ######## ##?######"},
		{"######?## #      # ##?######"},
		{"      ?   #      #   ?      "},
		{"######?## #      # ##?######"},
		{"######?## ######## ##?######"},
		{"######?##          ##?######"},
		{"######?## ######## ##?######"},
		{"######?## ######## ##?######"},
		{"#????????????##????????????#"},
		{"#?####?#####?##?#####?####?#"},
		{"#?####?#####?##?#####?####?#"},
		{"#+??##???????  ???????##??+#"},
		{"###?##?##?########?##?##?###"},
		{"###?##?##?########?##?##?###"},
		{"#??????##????##????##??????#"},
		{"#?##########?##?##########?#"},
		{"#?##########?##?##########?#"},
		{"#??????????????????????????#"},
		{"############################"},
	};
	char playfield[H][W] =
	{
		{"############################"},
		{"#????????????##????????????#"},
		{"#+####?#####?##?#####?####+#"},
		{"#?####?#####?##?#####?####?#"},
		{"#?####?#####?##?#####?####?#"},
		{"#??????????????????????????#"},
		{"#?####?##?########?##?####?#"},
		{"#?####?##?########?##?####?#"},
		{"#??????##????##????##??????#"},
		{"######?##### ## #####?######"},
		{"######?##### ## #####?######"},
		{"######?##          ##?######"},
		{"######?## ######## ##?######"},
		{"######?## #      # ##?######"},
		{"      ?   #      #   ?      "},
		{"######?## #      # ##?######"},
		{"######?## ######## ##?######"},
		{"######?##          ##?######"},
		{"######?## ######## ##?######"},
		{"######?## ######## ##?######"},
		{"#????????????##????????????#"},
		{"#?####?#####?##?#####?####?#"},
		{"#?####?#####?##?#####?####?#"},
		{"#+??##???????  ???????##??+#"},
		{"###?##?##?########?##?##?###"},
		{"###?##?##?########?##?##?###"},
		{"#??????##????##????##??????#"},
		{"#?##########?##?##########?#"},
		{"#?##########?##?##########?#"},
		{"#??????????????????????????#"},
		{"############################"},
	};

	char logic[H][W] =
	{
		{"############################"},
		{"#C   LIR    C##C    LIR   C#"},
		{"# ####D##### ## #####D#### #"},
		{"# #### ##### ## ##### #### #"},
		{"#U####U#####U##U#####U####U#"},
		{"#IR  LIRLIRLIRLIRLIRLIR  LI#"},
		{"#D####D##D########D##D####D#"},
		{"# ####U## ######## ##U#### #"},
		{"#C   LI##C  C##C  C##IR   C#"},
		{"######D##### ## #####D######"},
		{"###### #####U##U##### ######"},
		{"###### ##C LIRLIR C## ######"},
		{"###### ## ######## ## ######"},
		{"######U##U#      #U##U######"},
		{"T     IRLI#      #IRLI     T"},
		{"######D##D#      #D##D######"},
		{"###### ##U########U## ######"},
		{"###### ##IR      LI## ######"},
		{"###### ##D########D## ######"},
		{"######U##U########U##U######"},
		{"#C   LIRLIR C##C LIRLIR   C#"},
		{"# ####D##### ## #####D#### #"},
		{"# ####U#####U##U#####U#### #"},
		{"#C C##IRLIRLIRLIRLIRLI##C C#"},
		{"### ##D##D########D##D## ###"},
		{"###U## ## ######## ## ##U###"},
		{"#CLIR C##C  C##C  C##C LIRC#"},
		{"# ########## ## ########## #"},
		{"# ##########U##U########## #"},
		{"#C         LIRLIR         C#"},
		{"############################"},
	};
};