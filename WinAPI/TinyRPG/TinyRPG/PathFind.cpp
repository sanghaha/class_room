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

		// �̵� �Ϸ��ߴ��� Ȯ��
		//if (_path[0].ConvertToPos() == _owner->GetPos())
		if(false == result)
		{
			_currPathCount++;
			// �ش� path�� �̵��� �Ϸ�Ǿ���. �÷��̾� ��ġ�� ����Ǿ����� ���� �ѹ��� ã�´�.
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

	// �ʹ� ���� ã���� ���� ������ ������ ����
	if (_currPathCount == 0 || _currPathCount == _checkPathCount)
	{
		// path�� ã�Ƽ� path��� �̵��Ѵ�.
		Cell start = _owner->GetPosCell();
		Cell end = _getDestFunc ? _getDestFunc() : _destCell;
		Game::GetGameScene()->FindPath(start, end, _path);

		// ���� ã�Ҵ�
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
