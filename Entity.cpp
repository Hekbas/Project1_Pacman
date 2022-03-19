#include "Game.h"
//EJEMPLO
Entity::Entity()
{
	is_alive = false;
}
Entity::~Entity()
{
}
void Entity::Init(int posx, int posy, int w, int h, int s)
{
	x = posx;
	y = posy;
	width = w;
	height = h;
	speed = s;
	is_alive = true;
}
void Entity::InitPacman(int x, int y, int vx, int vy, int vxTurn, int vyTurn, bool is_alive)
{
	this->x = x;
	this->y = y;
	this->vx = vx;
	this->vy = vy;
	this->vxTurn = vxTurn;
	this->vyTurn = vyTurn;
	this->is_alive = is_alive;

	width = 32;
	height = 32;
}
void Entity::InitGhost(int x, int y, int vx, int vy, bool is_alive, bool chase, char posOld)
{
	this->x = x;
	this->y = y;
	this->vx = vx;
	this->vy = vy;
	this->is_alive = is_alive;
	this->chase = chase;
	this->posOld = posOld;

	width = 32;
	height = 32;
}
void Entity::GetRect(int *posx, int *posy, int *w, int *h)
{
	*posx = x;
	*posy = y;
	*w = width;
	*h = height;
}
void Entity::GetRectPacman(int* posx, int* posy, int* w, int* h)
{
	*posx = x*32;
	*posy = y*32;
	*w = width;
	*h = height;
}

int Entity::GetX() { return x; }
void Entity::SetX(int x) { this->x = x; }

int Entity::GetY() { return y; }
void Entity::SetY(int y) { this->y = y; }

int Entity::GetVx() { return vx; }
void Entity::SetVx(int vx) { this->vx = vx; }

int Entity::GetVy() { return vy; }
void Entity::SetVy(int vy) { this->vy = vy; }

int Entity::GetVxTurn() { return vxTurn; }
void Entity::SetVxTurn(int vxTurn) { this->vxTurn = vxTurn; }

int Entity::GetVyTurn() { return vyTurn; }
void Entity::SetVyTurn(int vyTurn) { this->vyTurn = vyTurn; }

bool Entity::GetChase() { return chase; }

char Entity::GetPosOld() { return posOld; }
void Entity::SetPosOld(char posOld) { this->posOld = posOld; }

int Entity::GetWidth() { return width; }

void Entity::ShutDown() { is_alive = false; }
bool Entity::IsAlive() { return is_alive; }
void Entity::Move(int dx, int dy)
{
	x += dx * speed;
	y += dy * speed;
}