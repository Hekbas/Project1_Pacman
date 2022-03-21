#include "Game.h"

#include <math.h>
#include <iostream>
#include <stdlib.h>

Game::Game() {}
Game::~Game() {}

Game::Game(char playfield[H][W])
{
	this->playfield[H][W] = playfield[H][W];
}


bool Game::Init()
{
	//Initialize SDL with all subsystems
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	//Create window (title, x, y, w, h, flags)
	Window = SDL_CreateWindow("PacMan Minigame - NoSus Studios", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if (Window == NULL)
	{
		SDL_Log("Unable to create window: %s", SDL_GetError());
		return false;
	}

	//Create renderer (window, device index, flags)
	Renderer = SDL_CreateRenderer(Window, -1, 0);
	if (Renderer == NULL)
	{
		SDL_Log("Unable to create rendering context: %s", SDL_GetError());
		return false;
	}

	//Initialize keys array
	for (int i = 0; i < MAX_KEYS; ++i)
		keys[i] = KEY_IDLE;

	//Load images
	if (!LoadImages())
		return false;

	//Load sound
	if (LoadSound() == false) { return false; }

	Mix_Volume(-1, MIX_MAX_VOLUME / 2);
	
	//Init variables
	Status.Status(0, 2, true, 0, 0);

	Pacman.InitPacman( 13, 23, 0, 0, -1, 0, true);
	GhostRed.InitGhost(13, 11, -1, 0, true, true, NULL);

	int w;
	SDL_QueryTexture(img_background, NULL, NULL, &w, NULL);
	Scene.Init(0, 0, w, WINDOW_HEIGHT, 0);

	for (int i = 0; i < H; i++)
	{
		for (int j = 0; j < W; j++)
		{
			Food[i][j].Init(32 * i, 32 * j, 32, 32, 10);
		}
	}

	return true;
}

bool Game::LoadSound() {
	if (Mix_Init(MIX_INIT_OGG) != MIX_INIT_OGG) {
		SDL_Log("Mix_Init, failed to init required ogg support: %s\n", Mix_GetError());
		return false;
	}

	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1034) == -1) {
		SDL_Log("Mix_Init, failed to open the audio: %s\n", Mix_GetError());
		return false;
	}

	//PACMAN SOUND
	gamestartsound = Mix_LoadWAV("gamestartsound.wav");
	if (gamestartsound == NULL) {
		SDL_Log("Mix_LoadWAV failed: %s\n", Mix_GetError());
		return false;
	}

	deathsound = Mix_LoadWAV("deathsound.wav");
	if (deathsound == NULL) {
		SDL_Log("Mix_LoadWAV failed: %s\n", Mix_GetError());
		return false;
	}

	wakasound_1 = Mix_LoadWAV("munch_1.wav");
	if (wakasound_1 == NULL) {
		SDL_Log("Mix_LoadWAV failed: %s\n", Mix_GetError());
		return false;
	}

	wakasound_2 = Mix_LoadWAV("munch_2.wav");
	if (wakasound_2 == NULL) {
		SDL_Log("Mix_LoadWAV failed: %s\n", Mix_GetError());
		return false;
	}

	eatfruitsound = Mix_LoadWAV("eatfruitsound.wav");
	if (eatfruitsound == NULL) {
		SDL_Log("Mix_LoadWAV failed: %s\n", Mix_GetError());
		return false;
	}


	eatghostsound = Mix_LoadWAV("eatghostsound.wav");
	if (eatghostsound == NULL) {
		SDL_Log("Mix_LoadWAV failed: %s\n", Mix_GetError());
		return false;
	}

	powerpelletsound = Mix_LoadWAV("powerpelletsound.wav");
	if (powerpelletsound == NULL) {
		SDL_Log("Mix_LoadWAV failed: %s\n", Mix_GetError());
		return false;
	}
	return true;
}

bool Game::LoadImages()
{
	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
	{
		SDL_Log("IMG_Init, failed to init required png support: %s\n", IMG_GetError());
		return false;
	}
	img_menu = SDL_CreateTextureFromSurface(Renderer, IMG_Load("menu_screen.png"));
	if (img_menu == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_gameover = SDL_CreateTextureFromSurface(Renderer, IMG_Load("gameover.png"));
	if (img_gameover == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_background = SDL_CreateTextureFromSurface(Renderer, IMG_Load("background.png"));
	if (img_background == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	pacman_up = SDL_CreateTextureFromSurface(Renderer, IMG_Load("Sprites/pacman/pacman_up.png"));
	if (pacman_up == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	pacman_down = SDL_CreateTextureFromSurface(Renderer, IMG_Load("Sprites/pacman/pacman_down.png"));
	if (pacman_down == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	pacman_left = SDL_CreateTextureFromSurface(Renderer, IMG_Load("Sprites/pacman/pacman_left.png"));
	if (pacman_left == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	pacman_right = SDL_CreateTextureFromSurface(Renderer, IMG_Load("Sprites/pacman/pacman_right.png"));
	if (pacman_right == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	pacman_birth = SDL_CreateTextureFromSurface(Renderer, IMG_Load("Sprites/pacman/pacman_birth.png"));
	if (pacman_birth == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_food = SDL_CreateTextureFromSurface(Renderer, IMG_Load("food.png"));
	if (img_food == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_powerup = SDL_CreateTextureFromSurface(Renderer, IMG_Load("powerup.png"));
	if (img_powerup == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_ghostRed = SDL_CreateTextureFromSurface(Renderer, IMG_Load("Sprites/red/red_down.png"));
	if (img_ghostRed == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_life = SDL_CreateTextureFromSurface(Renderer, IMG_Load("Sprites/pacman/pacman_left.png"));
	if (img_life == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_score = SDL_CreateTextureFromSurface(Renderer, IMG_Load("numbers.png"));
	if (img_score == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_scaredBlue = SDL_CreateTextureFromSurface(Renderer, IMG_Load("Sprites/hunt/scared_blue.png"));
	if (img_scaredBlue == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}

	img_pacman = pacman_birth;
	img_ghost = img_ghostRed;

	return true;
}

void Game::Release()
{
	//Release images
	SDL_DestroyTexture(img_background);
	SDL_DestroyTexture(pacman_up);
	SDL_DestroyTexture(pacman_down);
	SDL_DestroyTexture(pacman_left);
	SDL_DestroyTexture(pacman_right);
	SDL_DestroyTexture(pacman_birth);
	SDL_DestroyTexture(img_food);
	SDL_DestroyTexture(img_menu);
	SDL_DestroyTexture(img_gameover);
	SDL_DestroyTexture(img_score);
	SDL_DestroyTexture(img_life);
	IMG_Quit();

	// Free Audios
	Mix_FreeChunk(gamestartsound);
	Mix_FreeChunk(wakasound_1);
	Mix_FreeChunk(wakasound_2);
	Mix_FreeChunk(deathsound);
	Mix_FreeChunk(eatfruitsound);
	Mix_FreeChunk(eatghostsound);
	Mix_FreeChunk(powerpelletsound);

	// Close SDL_Mixer
	Mix_CloseAudio();
	while (Mix_Init(0))
		Mix_Quit();

	//Clean up all SDL initialized subsystems
	SDL_Quit();
}

bool Game::Input()
{
	SDL_Event event;
	if (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)	return false;
	}

	SDL_PumpEvents();
	const Uint8* keyboard = SDL_GetKeyboardState(NULL);
	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keyboard[i])
			keys[i] = (keys[i] == KEY_IDLE) ? KEY_DOWN : KEY_REPEAT;
		else
			keys[i] = (keys[i] == KEY_REPEAT || keys[i] == KEY_DOWN) ? KEY_UP : KEY_IDLE;
	}

	return true;
}


void Game::Frightened()
{
	if (Status.GetFrightened() > 0)
	{
		Status.SetFrightened(Status.GetFrightened()-1);
		img_ghost = img_scaredBlue;
	}
	else
	{
		img_ghost = img_ghostRed;
	}
}

bool Game::CheckForDeath()
{
	int x = Pacman.GetX();
	int y = Pacman.GetY();

	int xG = GhostRed.GetX();
	int yG = GhostRed.GetY();

	if (x == xG && y == yG)
	{
		GhostRed.SetX(13);
		GhostRed.SetY(11);
		GhostRed.SetVx(-1);
		GhostRed.SetVy(0);

		if (Status.GetFrightened() > 0)
		{
			Status.SetFrightened(0);
		}
		else
		{
			img_pacman = pacman_up;
			Status.SetLives(Status.GetLives() - 1);

			Pacman.SetX(13);
			Pacman.SetY(23);
			Pacman.SetVx(-1);
			Pacman.SetVy(0);
			Pacman.SetVxTurn(-1);
			Pacman.SetVyTurn(0);

			return true;
		}
	}

	return false;
}

void Game::ResetVariables()
{
	//reset playfield
	for (int i = 0; i < H; i++)
	{
		for (int j = 0; j < W; j++)
		{
			playfield[i][j] = playfieldOriginal[i][j];
		}
	}

	//reset pacaman nice fresco $3xo
	img_pacman = pacman_birth;

	//lives an points
	Status.SetLives(2);
	Status.SetScore(0);

	Pacman.SetVxTurn(-1);
	Pacman.SetVyTurn(0);

	Status.SetGameOverR(0);
}

void Game::Logic_Pacman()
{
	int x = Pacman.GetX();
	int y = Pacman.GetY();
	int vx = Pacman.GetVx();
	int vy = Pacman.GetVy();
	int vxTurn = Pacman.GetVxTurn();
	int vyTurn = Pacman.GetVyTurn();

	int xG = GhostRed.GetX();
	int yG = GhostRed.GetY();

	// delete PacMan from old position
	playfield[y][x] = ' ';

	if (x == 0 && y == 14 && vx == -1)  //left teleport
		x = 27;
	else if (x == 27 && y == 14 && vx == 1)  //right teleport
		x = 0;
	else
	{
		// new coordinates (turn)
		int xTurn = vxTurn + x;
		int yTurn = vyTurn + y;
		// new coordinates (straight)
		int xStraight = vx + x;
		int yStraight = vy + y;

		if (playfield[yTurn][xTurn] != '#')    //try turning
		{
			vx = vxTurn;
			vy = vyTurn;
		}
		else if (playfield[yStraight][xStraight] == '#')    //try going straight
		{
			vx = 0;
			vy = 0;
		}

		// update PacMan's coordinate
		x += vx;
		y += vy;

		// what's in the new location?
		if (playfield[y][x] == -73)
		{
			Status.SetScore(Status.GetScore() + 10);

			if ((Status.GetScore()/10) % 2 == 0)
				Mix_PlayChannel(-1, wakasound_1, 0);
			else
				Mix_PlayChannel(-1, wakasound_2, 0);
		}
		else if (playfield[y][x] == '+')
		{
			Status.SetScore(Status.GetScore() + 50);
			Status.SetFrightened(80);

			Mix_PlayChannel(-1, powerpelletsound, 3);
		}
	}
	// change xy
	Pacman.SetX(x);
	Pacman.SetY(y);
	Pacman.SetVx(vx);
	Pacman.SetVy(vy);

	Pacman.SetVxTurn(vxTurn);
	Pacman.SetVyTurn(vyTurn);
}

void Game::Logic_Ghost()
{
	int x = GhostRed.GetX();
	int y = GhostRed.GetY();
	int vx = GhostRed.GetVx();
	int vy = GhostRed.GetVy();
	bool chase = GhostRed.GetChase();
	char posOld = GhostRed.GetPosOld();

	int xP = Pacman.GetX();
	int yP = Pacman.GetY();

	char rPath = 'N';
	bool rExists = false;

	if (Status.GetFrightened() > 0)
	{
		srand(time(NULL));

		if (logic[y][x] == 'I')  //chek for intersections
		{
			//Choose random path
			

			while (rExists == false)
			{
				int rNumber = rand() % ((4 + 1) - 1) + 1;
				
				switch (rNumber)
				{
				case 1: rPath = 'L'; break;
				case 2: rPath = 'R'; break;
				case 3: rPath = 'U'; break;
				case 4: rPath = 'D'; break;
				default: break;
				}

				for (int i = y - 1; i < y + 2; i++)
				{
					for (int j = x - 1; j < x + 2; j++)
					{
						if (logic[i][j] == rPath && logic[i][j] != posOld)
						{
							rExists = true;
							break;
						}
					}
					if (rExists == true) break;
				}
			}

			//change ghost speed to follow "random" path
			switch (rPath)
			{
			case 'L': GhostRed.SetVx(-1); GhostRed.SetVy(0); break;
			case 'R': GhostRed.SetVx(1); GhostRed.SetVy(0);  break;
			case 'U': GhostRed.SetVx(0); GhostRed.SetVy(-1); break;
			case 'D': GhostRed.SetVx(0); GhostRed.SetVy(1);  break;
			default: break;
			}
		}
		else if (logic[y][x] == 'C')  //chek for corners
		{
			if (vx != 0)
			{
				GhostRed.SetVx(0);

				if (logic[y - 1][x] != '#') //check UP
					GhostRed.SetVy(-1);
				else
					GhostRed.SetVy(1);
			}
			else
			{
				GhostRed.SetVy(0);

				if (logic[y][x - 1] != '#') //check LEFT
					GhostRed.SetVx(-1);
				else
					GhostRed.SetVx(1);
			}
		}
		else    //set actual pos as posOld
		{
			switch (logic[y][x])
			{
			case 'L': GhostRed.SetPosOld('L');  break;
			case 'R': GhostRed.SetPosOld('R');  break;
			case 'U': GhostRed.SetPosOld('U');  break;
			case 'D': GhostRed.SetPosOld('D');  break;
			default: break;
			}
		}
	}
	else
	{
		if (logic[y][x] == 'I')  //chek for intersections
		{
			float distMin = 100;
			char bestPath;

			for (int i = y - 1; i < y + 2; i++)
			{
				for (int j = x - 1; j < x + 2; j++)
				{
					//check for paths 1 tyle around, excluding the one it came from
					if ((logic[i][j] == 'L' ||
						logic[i][j] == 'R' ||
						logic[i][j] == 'U' ||
						logic[i][j] == 'D') &&
						logic[i][j] != posOld)
					{
						int xDelta;
						int yDelta;

						//find wich path has the shortest distance (straight line) to pacMan
						if (chase == true)
						{
							xDelta = i - yP;
							yDelta = j - xP;
						}
						else
						{
							xDelta = i - 0;
							yDelta = j - 27;
						}

						float dist = hypot(xDelta, yDelta);

						if (dist < distMin)
						{
							distMin = dist;
							bestPath = logic[i][j];
						}
					}
				}
			}

			//change ghost speed to follow "best" path
			switch (bestPath)
			{
			case 'L': GhostRed.SetVx(-1); GhostRed.SetVy(0); break;
			case 'R': GhostRed.SetVx(1); GhostRed.SetVy(0);  break;
			case 'U': GhostRed.SetVx(0); GhostRed.SetVy(-1); break;
			case 'D': GhostRed.SetVx(0); GhostRed.SetVy(1);  break;
			default: break;
			}
		}
		else if (logic[y][x] == 'C')  //chek for corners
		{
			if (vx != 0)
			{
				GhostRed.SetVx(0);

				if (logic[y - 1][x] != '#') //check UP
					GhostRed.SetVy(-1);
				else
					GhostRed.SetVy(1);
			}
			else
			{
				GhostRed.SetVy(0);

				if (logic[y][x - 1] != '#') //check LEFT
					GhostRed.SetVx(-1);
				else
					GhostRed.SetVx(1);
			}
		}
		else    //set actual pos as posOld
		{
			switch (logic[y][x])
			{
			case 'L': GhostRed.SetPosOld('L');  break;
			case 'R': GhostRed.SetPosOld('R');  break;
			case 'U': GhostRed.SetPosOld('U');  break;
			case 'D': GhostRed.SetPosOld('D');  break;
			default: break;
			}
		}

	}

	// update ghost pos
	x += GhostRed.GetVx();
	y += GhostRed.GetVy();

	GhostRed.SetX(x);
	GhostRed.SetY(y);
}


bool Game::UpdateMenu()
{
	if (!Input())	return true;
	//EXIT
	if (keys[SDL_SCANCODE_ESCAPE] == KEY_DOWN)
	{
		continuemenu = false;
		return true;
	}
	//COME IN
	if (keys[SDL_SCANCODE_SPACE] == KEY_DOWN)
	{
		continuemenu = true;
		return false;
	}

	continuemenu = false;
	return false;
}

bool Game::GetContinueMenu()
{
	return continuemenu;
}


void Game::startIntro()
{
	Draw();
	Mix_PlayChannel(-1, gamestartsound, 0);
	SDL_Delay(4000);
}

bool Game::Update()
{
	//Read Input
	if (!Input())	return true;

	//Process Input
	int fx = 0, fy = 0;
	if (keys[SDL_SCANCODE_ESCAPE] == KEY_DOWN)	return true;
	if (keys[SDL_SCANCODE_F1] == KEY_DOWN)		god_mode = !god_mode;
	if (keys[SDL_SCANCODE_UP] == KEY_DOWN)
	{
		Pacman.SetVxTurn(0);
		Pacman.SetVyTurn(-1);
		img_pacman = pacman_up;
	}
	if (keys[SDL_SCANCODE_DOWN] == KEY_DOWN)
	{
		Pacman.SetVxTurn(0);
		Pacman.SetVyTurn(+1);
		img_pacman = pacman_down;
	}
	if (keys[SDL_SCANCODE_LEFT] == KEY_DOWN)
	{
		Pacman.SetVxTurn(-1);
		Pacman.SetVyTurn(0);
		img_pacman = pacman_left;
	}
	if (keys[SDL_SCANCODE_RIGHT] == KEY_DOWN)
	{
		Pacman.SetVxTurn(+1);
		Pacman.SetVyTurn(0);
		img_pacman = pacman_right;
	}

	//lives = 0?
	if (Status.GetLives() == 0)
	{
		return true;
	}

	//Eaten all dots?
	for (int i = 0; i < H; i++)
	{
		for (int j = 0; j < W; j++)
		{
			if (playfield[i][j] == -73)
				return false;
		}
	}
	return true;
}

void Game::GetRect2(int* x, int* y, int* w, int* h)
{
	*y = 0;
	*w = 32;
	*h = 32;

	bool state = (SDL_GetTicks() / 100) % 2;

	if (state == 0)
		*x = *w * 0;

	if (state == 1)
		*x = *w * 1;
}

void Game::GetRect3(int* posx, int* posy, int* w, int* h, int x, int y, int _w, int _h) {
	*posx = x;
	*posy = y;
	*w = _w;
	*h = _h;
}

void Game::DrawMenu()
{
	SDL_Rect destRC;
	SDL_Rect srcRC;

	
	SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
	
	SDL_RenderClear(Renderer);
	
	Scene.GetRect(&destRC.x, &destRC.y, &destRC.w, &destRC.h);
	SDL_RenderCopy(Renderer, img_menu, NULL, &destRC);

	SDL_RenderPresent(Renderer);
}

bool Game::GameOver() {
	SDL_Rect destRC;
	SDL_Rect srcRC;

	bool state = (SDL_GetTicks() / 100) % 2;

	if (state == 1) {
		Status.AddGameOverR();
	}

	if(Status.GetGameOverR()==800) {
		return true;
	}
	
	
	SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
	SDL_RenderClear(Renderer);

	if (((SDL_GetTicks() / 100) % 2) == 0) {
		GetRect3(&destRC.x, &destRC.y, &destRC.w, &destRC.h, 0, (792 / 2), 896, 200);
		SDL_RenderCopy(Renderer, img_gameover, NULL, &destRC);
	}

	SDL_RenderPresent(Renderer);
	
	return false;
}

void Game::Draw()
{
	SDL_Rect destRC;
	SDL_Rect srcRC;


	//SDL_RenderSetScale(Renderer, 2, 2);

	//Set the color used for drawing operations
	SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
	//Clear rendering target
	SDL_RenderClear(Renderer);

	////God mode uses red wireframe rectangles for physical objects
	//if (god_mode) SDL_SetRenderDrawColor(Renderer, 192, 0, 0, 255);

	//Draw scene
	Scene.GetRect(&destRC.x, &destRC.y, &destRC.w, &destRC.h);
	SDL_RenderCopy(Renderer, img_background, NULL, &destRC);
	/*destRC.x += destRC.w;
	SDL_RenderCopy(Renderer, img_background, NULL, &destRC);*/

	//Draw Food
	for (int i = 0; i < H; i++)
	{
		for (int j = 0; j < W; j++)
		{
			if (playfield[i][j] == -73)
			{
				Food[i][j].GetRect(&destRC.y, &destRC.x, &destRC.w, &destRC.h);
				SDL_RenderCopy(Renderer, img_food, NULL, &destRC);
			}
			else if (playfield[i][j] == '+')
			{
				Food[i][j].GetRect(&destRC.y, &destRC.x, &destRC.w, &destRC.h);
				SDL_RenderCopy(Renderer, img_powerup, NULL, &destRC);
			}
		}
	}

	//Ghost
	GhostRed.GetRectPacman(&destRC.x, &destRC.y, &destRC.w, &destRC.h);
	GetRect2(&srcRC.x, &srcRC.y, &srcRC.w, &srcRC.h);
	SDL_RenderCopy(Renderer, img_ghost, &srcRC, &destRC);

	//Draw Pacman
	Pacman.GetRectPacman(&destRC.x, &destRC.y, &destRC.w, &destRC.h);
	GetRect2(&srcRC.x, &srcRC.y, &srcRC.w, &srcRC.h);
	SDL_RenderCopy(Renderer, img_pacman, &srcRC, &destRC);
	//if (god_mode) SDL_RenderDrawRect(Renderer, &rc);

	//Draw lives
	if (Status.GetLives() > 1) {
		GetRect3(&destRC.x, &destRC.y, &destRC.w, &destRC.h, 512, 448, 32, 32);
		GetRect3(&srcRC.x, &srcRC.y, &srcRC.w, &srcRC.h, 0, 0, 32, 32);
		SDL_RenderCopy(Renderer, img_life, &srcRC, &destRC);
	}
	int uW = 0, tW = 0, hW = 0, thW = 0;

	int score = Status.GetScore();

	uW = (score % 10) * 32;
	score = score / 10;
	tW = (score % 10) * 32;
	score = score / 10;
	hW = (score % 10) * 32;
	score = score / 10;
	thW = (score % 10) * 32;

	GetRect3(&destRC.x, &destRC.y, &destRC.w, &destRC.h, 512, 480, 32, 32);
	GetRect3(&srcRC.x, &srcRC.y, &srcRC.w, &srcRC.h, uW, 0, 32, 32);
	SDL_RenderCopy(Renderer, img_score, &srcRC, &destRC);

	GetRect3(&destRC.x, &destRC.y, &destRC.w, &destRC.h, 480, 480, 32, 32);
	GetRect3(&srcRC.x, &srcRC.y, &srcRC.w, &srcRC.h, tW, 0, 32, 32);
	SDL_RenderCopy(Renderer, img_score, &srcRC, &destRC);

	GetRect3(&destRC.x, &destRC.y, &destRC.w, &destRC.h, 448, 480, 32, 32);
	GetRect3(&srcRC.x, &srcRC.y, &srcRC.w, &srcRC.h, hW, 0, 32, 32);
	SDL_RenderCopy(Renderer, img_score, &srcRC, &destRC);

	GetRect3(&destRC.x, &destRC.y, &destRC.w, &destRC.h, 416, 480, 32, 32);
	GetRect3(&srcRC.x, &srcRC.y, &srcRC.w, &srcRC.h, thW, 0, 32, 32);
	SDL_RenderCopy(Renderer, img_score, &srcRC, &destRC);

	//Update screen
	SDL_RenderPresent(Renderer);
}