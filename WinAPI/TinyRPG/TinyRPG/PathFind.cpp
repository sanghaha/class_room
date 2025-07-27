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
		if (_owner->IsCompleteMove())
		//if(false == result)
		{
			PrintLog(std::format(L"PathFind::Update(){0} : {1},{2}, count:{3}", (int64)this, _owner->GetPosCell().index_X, _owner->GetPosCell().index_Y, (int32)_currPathCount));

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
	if (_currPathCount == 0 || _currPathCount >= _checkPathCount)
	{
		// path�� ã�Ƽ� path��� �̵��Ѵ�.
		Cell start = _owner->GetPosCell();
		Cell end = _getDestFunc ? _getDestFunc() : _destCell;
		Game::GetGameScene()->FindPath(start, end, _path);
		_originpath = _path;

		// ���� ã�Ҵ�
		if (_path.size() != 0)
		{
			_destCell = _path[_path.size() - 1];
		}
	}
	else
	{
		++_currPathCount;
		PrintLog(std::format(L"(1)PathFind::Move(){0}  :{1},{2}", (int64)this,(int32)_currPathCount, (int32)_path.size()));

		if (_path.size() != 0)
		{
			_path.erase(_path.begin());
		}
	}

	if (_path.size() != 0)
	{
		PrintLog(std::format(L"PathFind::Move(){0}  :{1},{2} -> {3},{4} ", (int64)this, _owner->GetPosCell().index_X, _owner->GetPosCell().index_Y, _path[0].index_X, _path[0].index_Y));

		_owner->Move(_path[0]);
	}
}
