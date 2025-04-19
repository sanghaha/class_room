#include "pch.h"
#include "Actor.h"
#include "Scene.h"

Actor::Actor(Pos pos) : _pos(pos)
{
}

Actor::~Actor()
{
}

void Actor::Init()
{
	Scene::GetInstance()->UpdateGrid(this, Pos{ 0,0 }, _pos);
}

void Actor::AddPosDelta(float x, float y, bool notifyScene)
{
	Pos prevPos = _pos;

	_pos.x += x;
	_pos.y += y;

	if (notifyScene)
	{
		// Scene에 알려준다.
		Scene::GetInstance()->UpdateGrid(this, prevPos, _pos);
	}
}

void Actor::SetPos(Pos pos, bool notifyScene)
{
	Pos prevPos = _pos;

	_pos = pos;

	if (notifyScene)
	{
		// Scene에 알려준다.
		Scene::GetInstance()->UpdateGrid(this, prevPos, _pos);
	}
}
