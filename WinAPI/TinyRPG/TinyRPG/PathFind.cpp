#include "pch.h"
#include "PathFind.h"
#include "Creature.h"
#include "Game.h"
#include "GameScene.h"

void PathFind::Start(Creature* owner, Cell start, Cell dest, int8 refreshCount, std::function<Cell()> getDestFunc)
{
	_owner = owner;
	_currPathCount = 0;
	_checkPathCount = refreshCount;
	_destCell = dest;
	_getDestFunc = getDestFunc;

	calcPath();
}

void PathFind::Update(float deltaTime)
{
	if (_owner == nullptr)
		return;

	if (_path.size() == 0)
	{
		calcPath();
	}
	else
	{
		bool result = _owner->AddPosDelta(deltaTime);

		// 이동 완료했는지 확인
		//if (_path[0].ConvertToPos() == _owner->GetPos())
		if(false == result)
		{
			_currPathCount++;
			// 해당 path로 이동이 완료되었다. 플레이어 위치가 변경되었으면 길을 한번더 찾는다.
			calcPath();
		}
	}
}

bool PathFind::IsComplete()
{
	if (_owner == nullptr)
		return true;

	if (_destCell.ConvertToPos() == _owner->GetPos())
		return true;

	if (_path.size() == 0)
		return true;

	return false;
}

void PathFind::calcPath()
{
	if (nullptr == _owner)
		return;

	// 너무 자주 찾으면 성능 문제가 있으니 조절
	if (_currPathCount == 0 || _currPathCount == _checkPathCount)
	{
		// path를 찾아서 path대로 이동한다.
		Cell start = _owner->GetPosCell();
		Cell end = _getDestFunc ? _getDestFunc() : _destCell;
		Game::GetGameScene()->FindPath(start, end, _path);

		// 길을 찾았다
		if (_path.size() != 0)
		{
			_destCell = _path[_path.size() - 1];
		}
	}
	else
	{
		++_currPathCount;

		if (_path.size() != 0)
		{
			_path.erase(_path.begin());
		}
	}

	if (_path.size() != 0)
	{
		int32 xDir = _path[0].index_X - _owner->GetPosCell().index_X;
		int32 yDir = _path[0].index_Y - _owner->GetPosCell().index_Y;
		_owner->Move(xDir, yDir);
	}
}
