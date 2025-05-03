#include "pch.h"
#include "Actor.h"
#include "Scene.h"
#include "Game.h"
#include "GameScene.h"

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
	Game::GetScene()->UpdateGrid(this, Cell{ 0,0 }, _posCell);
}

void Actor::Destory()
{
	// 사라져야함
	Game::GetGameScene()->ReserveRemove(this);
}

bool Actor::AddPosDelta(float deltaTime)
{
	Vector destPos = _posCell.ConvertToPos();
	Vector dir = destPos - _pos;
	dir.Normalize();

	float x = dir.x * _moveSpeed * deltaTime;
	float y = dir.y * _moveSpeed * deltaTime;

	Vector prevPos = _pos;
	Vector nextPos = _pos + Vector(x, y);

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
		return false;
	}
	else
	{
		_pos = nextPos;
		return true;
	}
}

void Actor::SetPos(Vector pos, bool notifyScene)
{
	Cell prevCell = _posCell;

	_pos = pos;
	_posCell = Cell::ConvertToCell(pos);

	if (notifyScene)
	{
		// Scene에 알려준다.
		Game::GetScene()->UpdateGrid(this, prevCell, _posCell);
	}
}

void Actor::SetPosCell(Cell cell, bool notifyScene)
{
	Cell prevCell = _posCell;
	_posCell = cell;

	if (notifyScene)
	{
		// Scene에 알려준다.
		Game::GetScene()->UpdateGrid(this, prevCell, _posCell);
	}
}

bool Actor::IsCulling() const
{
	return Game::GetScene()->IsCulling(_pos);
}
