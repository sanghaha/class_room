#include "pch.h"
#include "Actor.h"
#include "Scene.h"
#include "Game.h"

Actor::Actor(Vector pos)
{
	_posCell = Cell::ConvertToCell(pos);
	_pos = _posCell.ConvertToPos();
}

Actor::~Actor()
{
}

void Actor::Init()
{
	Game::GetScene()->UpdateGrid(this, Vector{ 0,0 }, _pos);
}

void Actor::AddPosDelta(float x, float y, bool notifyScene)
{
	Vector prevPos = _pos;
	Vector nextPos = _pos + Vector(x, y);
	Vector destPos = _posCell.ConvertToPos();

	Vector toPrev = prevPos - destPos;
	toPrev.Normalize();
	Vector toCurrent = nextPos - destPos;
	toCurrent.Normalize();

	float dot = toPrev.Dot(toCurrent);
	//PrintLog(std::format(L"dest:{0},{1}, prev:{2},{3}, next:{4},{5}, dot:{6}, index:{7},{8}",
	//	(int32)destPos.x, (int32)destPos.y,
	//	(int32)prevPos.x, (int32)prevPos.y,
	//	(int32)nextPos.x, (int32)nextPos.y,
	//	dot,
	//	_posCell.index_X, _posCell.index_Y));

	if (dot <= 0)
	{
		_pos = destPos;
	}
	else
	{
		_pos = nextPos;
	}

	if (notifyScene)
	{
		// Scene에 알려준다.
		Game::GetScene()->UpdateGrid(this, prevPos, _pos);
	}
}

void Actor::SetPos(Vector pos, bool notifyScene)
{
	Vector prevPos = _pos;

	_pos = pos;
	_posCell = Cell::ConvertToCell(pos);

	if (notifyScene)
	{
		// Scene에 알려준다.
		Game::GetScene()->UpdateGrid(this, prevPos, _pos);
	}
}
