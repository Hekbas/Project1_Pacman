#pragma once

class Entity
{
public:
	Entity();
	~Entity();

	void Init(int posx, int posy, int w, int h, int s);

	void Status(int score, int lives, bool chase, int frightened, int GameOverR);

	void InitPacman(int x, int y, int vx, int vy, int vxTurn, int vyTurn, bool is_alive);
	void InitGhost(int x, int y, int vx, int vy, bool is_alive, bool chase, char posOld);

	void GetRect(int *posx, int *posy, int *w, int *h);
	void GetRectPacman(int* posx, int* posy, int* w, int* h);

	int GetX();
	void SetX(int x);

	int GetY();
	void SetY(int y);

	int GetVx();
	void SetVx(int vx);

	int GetVy();
	void SetVy(int vy);

	int GetVxTurn();
	void SetVxTurn(int vxTurn);

	int GetVyTurn();
	void SetVyTurn(int vyTurn);

	char GetPosOld();
	void SetPosOld(char posOld);

	int  GetWidth();

	int GetScore();
	void SetScore(int score);

	int GetLives();
	void SetLives(int lives);

	bool GetChase();
	void SetChase(bool chase);

	int GetFrightened();
	void SetFrightened(int frightened);

	int GetGameOverR();
	void SetGameOverR(int GameOverR);
	void AddGameOverR();


private:
	int x, y;
	int vx, vy;
	int vxTurn, vyTurn;

	int width, height;

	char posOld;

	int score;
	int lives;

	bool chase;
	int frightened;

	int GameOverR;
};