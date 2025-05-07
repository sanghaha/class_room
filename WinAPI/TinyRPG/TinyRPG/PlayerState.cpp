#include "pch.h"
#include "PlayerState.h"
#include "InputManager.h"
#include "Player.h"
#include "Game.h"
#include "Scene.h"
#include "GameScene.h"
#include "Map.h"

/// <summary>
/// 
/// </summary>
PlayerState_Idle::PlayerState_Idle(class Player* player) : Super(player)
{ 

}

PlayerState_Idle::~PlayerState_Idle()
{
}

AnimType PlayerState_Idle::GetAnimType()
{
	int32 xDir = InputManager::GetInstance()->GetMoveDirX();
	int32 yDir = InputManager::GetInstance()->GetMoveDirY();
	if (xDir != 0 || yDir != 0)
	{
		return AnimType::A_MOVE;
	}

	return AnimType::A_IDLE;
}

void PlayerState_Idle::Enter()
{
	if (nullptr == _player)
		return;
}

void PlayerState_Idle::Update(float deltaTime)
{
	if (nullptr == _player)
		return;

	int32 xDir = InputManager::GetInstance()->GetMoveDirX();
	int32 yDir = InputManager::GetInstance()->GetMoveDirY();

	if (xDir != 0)
	{
		_player->SetCurrDir(xDir > 0 ? DirType::DIR_RIGHT : DirType::DIR_LEFT);
	}
	if (yDir != 0)
	{
		_player->SetCurrDir(yDir > 0 ? DirType::DIR_DOWN : DirType::DIR_UP);
	}

	if (InputManager::GetInstance()->GetButtonPressed(KeyType::SpaceBar))
	{
		// Idle 상태에서만 공격 가능. 이동하면서 공격 불가능.
		_player->ChangeState(PlayerStateType::PS_ATTACK);
	}
	else if (InputManager::GetInstance()->GetButtonDown(KeyType::LeftMouse))
	{
		// 해당 위치로 길찾기
		POINT mousePos = InputManager::GetInstance()->GetMousePos();
		Vector pos = Game::ConvertWorldPos(Vector((float)mousePos.x, (float)mousePos.y));

		int32 indexX = (int32)pos.x / GTileSize;
		int32 indexY = (int32)pos.y / GTileSize;
		if (Map* map = Game::GetGameScene()->GetMap())
		{
			map->SetSelectedIndex(indexX, indexY);
			_player->ChangeState(PlayerStateType::PS_MOVE_PATH);
		}
	}
	else
	{

		if (xDir != 0)
		{
			_player->Move(xDir, 0);
			_player->ChangeState(PlayerStateType::PS_MOVE);
		}
		else if (yDir != 0)
		{
			_player->Move(0, yDir);
			_player->ChangeState(PlayerStateType::PS_MOVE);
		}
	}
}

/// <summary>
/// 
/// </summary>
PlayerState_Move::PlayerState_Move(class Player* player) : Super(player)
{
}

PlayerState_Move::~PlayerState_Move()
{
}

AnimType PlayerState_Move::GetAnimType()
{
	return AnimType::A_MOVE;
}

void PlayerState_Move::Enter()
{
	if (nullptr == _player)
		return;

	// dest 좌표값 계산
	Cell destCell = _player->GetPosCell();
	_destPos = destCell.ConvertToPos();

	//PrintLog(std::format(L"## Start MOVE : dest:{0},{1}, prev:{2},{3}",
	//	(int32)_destPos.x, (int32)_destPos.y,
	//	(int32)_prevPos.x, (int32)_prevPos.y));
}

void PlayerState_Move::Update(float deltaTime)
{
	if (nullptr == _player)
		return;

	// 방향 벡터 가져오기
	_player->AddPosDelta(deltaTime);
}

bool PlayerState_Move::IsEnd()
{
	// 목표지점에 도착했는지 체크
	Vector currentPos = _player->GetPos();

	//PrintLog(std::format(L"@@ Check MOVE : dest:{0},{1}, currentPos:{2},{3}",
	//	(int32)_destPos.x, (int32)_destPos.y,
	//	(int32)currentPos.x, (int32)currentPos.y));

	if (_destPos.x == currentPos.x && _destPos.y == currentPos.y)
	{
		//PrintLog(L"@@ Check MOVE ------------ END");

		return true;
	}
	//if (toPrev.Length() > 0.f && toCurrent.Length() > 0.f && toPrev.Dot(toCurrent) < 0)
	//{
	//	return true;
	//}

	return false;
}

/// <summary>
/// 
/// </summary>
PlayerState_Attack::PlayerState_Attack(class Player* player) : Super(player)
{
}

PlayerState_Attack::~PlayerState_Attack()
{
}

AnimType PlayerState_Attack::GetAnimType()
{
	return AnimType::A_ATTACK;
}

void PlayerState_Attack::Enter()
{
	if (nullptr == _player)
		return;

	_player->ResetAnimation(AnimType::A_ATTACK);
	
	_attackTime = _player->GetAttackTime();
}

void PlayerState_Attack::Update(float deltaTime)
{
	if (_attackTime != 0)
	{
		_attackTime -= deltaTime;

		if (_attackTime < 0)
		{
			_attackTime = 0;
			_player->Attack();
		}
	}
}

bool PlayerState_Attack::IsEnd()
{
	AnimInfo* animInfo = _player->GetCurrAnimation();
	if(animInfo)
		return animInfo->IsEnd;
	return true;
}

PlayerState_MovePath::PlayerState_MovePath(Player* player) : Super(player)
{
}

PlayerState_MovePath::~PlayerState_MovePath()
{
}

AnimType PlayerState_MovePath::GetAnimType()
{
	return AnimType::A_MOVE;
}

void PlayerState_MovePath::Enter()
{
	if (nullptr == _player)
		return;

	Map* map = Game::GetGameScene()->GetMap();
	if (map == nullptr)
		return;

	int32 destIndexX, destIndexY;
	map->GetSelectedIndex(destIndexX, destIndexY);

	Cell dest(destIndexX, destIndexY);
	_pathFind.Start(_player, _player->GetPosCell(), dest, 3, nullptr);

	Super::Enter();
}

void PlayerState_MovePath::Update(float deltaTime)
{
	Super::Update(deltaTime);
	_pathFind.Update(deltaTime);
}

bool PlayerState_MovePath::IsEnd()
{
	if (_pathFind.IsComplete())
	{
		Map* map = Game::GetGameScene()->GetMap();
		if (map)
		{
			map->ResetSelectedIndex();
		}
		return true;
	}

	return false;
}
