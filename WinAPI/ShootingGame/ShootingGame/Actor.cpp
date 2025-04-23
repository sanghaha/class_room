#include "pch.h"
#include "Actor.h"
#include "Scene.h"
#include "Game.h"

Actor::Actor(Pos pos) : _pos(pos)
{
}

Actor::~Actor()
{
}

void Actor::Init()
{
	Game::GetScene()->UpdateGrid(this, Pos{ 0,0 }, _pos);
}

void Actor::AddPosDelta(float x, float y, bool notifyScene)
{
	Pos prevPos = _pos;

	_pos.x += x;
	_pos.y += y;

	if (notifyScene)
	{
		// Scene에 알려준다.
		Game::GetScene()->UpdateGrid(this, prevPos, _pos);
	}
}

void Actor::SetPos(Pos pos, bool notifyScene)
{
	Pos prevPos = _pos;

	_pos = pos;

	if (notifyScene)
	{
		// Scene에 알려준다.
		Game::GetScene()->UpdateGrid(this, prevPos, _pos);
	}
}
