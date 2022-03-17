#pragma once

class Entity
{
public:
	Entity();
	~Entity();

	void Init(int posx, int posy, int w, int h, int s);
	void InitPacman(int x, int y, int vx, int vy, int vxTurn, int vyTurn, bool is_alive);
	void InitGhost(int x, int y, int vx, int vy, bool is_alive, bool chase, char posOld);

	void GetRect(int *posx, int *posy, int *w, int *h);
	void GetRectPacman(int* posx, int* posy, int* w, int* h);
	char GetType();

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

	bool GetChase();
	char GetPosOld();
	void SetPosOld(char posOld);

	int  GetWidth();
	void ShutDown(); 
	bool IsAlive();
	void Move(int dx, int dy);

private:
	int x, y;
	int vx, vy;
	int vxTurn, vyTurn;

	int width, height;
	int speed;
	bool is_alive;
	bool chase;

	char posOld;
};