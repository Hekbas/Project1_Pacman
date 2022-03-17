#include "Game.h"

#include <math.h>
#include <iostream>

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
	Window = SDL_CreateWindow("PacMan Minigame", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
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

	if (LoadSound() == false) { return false; }

	
	//Init variables
	//size: 104x82

	Pacman.InitPacman( 13, 23, 0, 0, -1, 0, true);
	GhostRed.InitGhost(13, 11, -1, 0, true, true, NULL);

	idx_shot = 0;
	int w;
	SDL_QueryTexture(img_background, NULL, NULL, &w, NULL);
	Scene.Init(0, 0, w, WINDOW_HEIGHT, 4);

	for (int i = 0; i < H; i++)
	{
		for (int j = 0; j < W; j++)
		{
			Food[i][j].Init(32 * i, 32 * j, 32, 32, 10);
		}
	}

	Mix_PlayChannel(-1, gamestartsound, 1);

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
	if (pacman_up == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	pacman_left = SDL_CreateTextureFromSurface(Renderer, IMG_Load("Sprites/pacman/pacman_left.png"));
	if (pacman_up == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	pacman_right = SDL_CreateTextureFromSurface(Renderer, IMG_Load("Sprites/pacman/pacman_right.png"));
	if (pacman_up == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	pacman_birth = SDL_CreateTextureFromSurface(Renderer, IMG_Load("Sprites/pacman/pacman_birth.png"));
	if (pacman_up == NULL) {
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

	img_pacman = pacman_birth;

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

void Game::Logic_Pacman()
{
	int x = Pacman.GetX();
	int y = Pacman.GetY();
	int vx = Pacman.GetVx();
	int vy = Pacman.GetVy();
	int vxTurn = Pacman.GetVxTurn();
	int vyTurn = Pacman.GetVyTurn();

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
		if (playfield[y][x] == '·')
		{
			//status.points += 10;

		}
		else if (playfield[y][x] == '+')
		{
			//status.points += 50;
			//status.frightened = true;
		}
	}
	// change xy
	Pacman.SetX(x);
	Pacman.SetY(y);
	Pacman.SetVx(vx);
	Pacman.SetVy(vy);
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
	

	if (x == xP && y == yP)  //eaten pacMan?
	{
		x = 13;
		y = 11;

		vx = -1;
		vy = 0;

		/*
		if (status.frightened == true)
		{
			status.frightened = false;
		}
		else
		{
			status.lives--;
			myPacMan.pos.x = 13;
			myPacMan.pos.y = 23;
		}
		*/
	}
	else if (logic[y][x] == 'I')  //chek for intersections
	{
		float distMin = 100;
		char bestPath;

		for (int i = y - 1; i < y + 2; i++)
		{
			for (int j = x - 1; j < x + 2; j++)
			{
				//check for paths 1 tyle around, excluding the one it came from
				if((logic[i][j] == 'L' ||
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

	// update ghost pos
	x += GhostRed.GetVx();
	y += GhostRed.GetVy();


	GhostRed.SetX(x);
	GhostRed.SetY(y);
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

	//Pacman update
	Pacman.Move(fx, fy);

	return false;
}

void Game::GetRect2(int* x, int* y, int* w, int* h) {
	*y = 0;
	*w = 32;
	*h = 32;
	bool state = (SDL_GetTicks() / 100) % 2;
	if (state == 0) {
		*x = *w * 0;
		Mix_PlayChannel(-1, wakasound_1, 0);
	}
	if (state == 1) {
		*x = *w * 1;
		Mix_PlayChannel(-1, wakasound_2, 0);
	}
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
	destRC.x += destRC.w;
	SDL_RenderCopy(Renderer, img_background, NULL, &destRC);

	//Draw Food
	for (int i = 0; i < H; i++)
	{
		for (int j = 0; j < W; j++)
		{
			if (playfield[i][j] == '·')
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
	SDL_RenderCopy(Renderer, img_ghostRed, &srcRC, &destRC);

	//Draw Pacman
	Pacman.GetRectPacman(&destRC.x, &destRC.y, &destRC.w, &destRC.h);
	GetRect2(&srcRC.x, &srcRC.y, &srcRC.w, &srcRC.h);
	SDL_RenderCopy(Renderer, img_pacman, &srcRC, &destRC);
	//if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
	
	
	//Update screen
	SDL_RenderPresent(Renderer);

	SDL_Delay(100);	// 1000/10 = 100 fps max
}