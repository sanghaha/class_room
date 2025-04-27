#include "pch.h"
#include "PlayerState.h"
#include "InputManager.h"
#include "Player.h"

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

	if (InputManager::GetInstance()->GetButtonPressed(KeyType::SpaceBar))
	{
		// Idle 상태에서만 공격 가능. 이동하면서 공격 불가능.
		_player->ChangeState(PlayerStateType::PS_ATTACK);
	}
	else
	{
		int32 xDir = InputManager::GetInstance()->GetMoveDirX();
		int32 yDir = InputManager::GetInstance()->GetMoveDirY();
		if (xDir != 0)
		{
			_player->Move(xDir, 0);
		}
		else if (yDir != 0)
		{
			_player->Move(0, yDir);
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
	float dirX = static_cast<float>(_player->GetDirX());
	float dirY = static_cast<float>(_player->GetDirY());

	// 방향 벡터의 길이 계산
	float length = sqrt(dirX * dirX + dirY * dirY);

	// 대각선 이동 시 벡터 정규화
	if (length > 0.0f)
	{
		dirX /= length;
		dirY /= length;
	}

	float x = dirX * _player->GetMoveSpeed() * deltaTime;
	float y = dirY * _player->GetMoveSpeed() * deltaTime;
	_player->AddPosDelta(x, y);
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
}

void PlayerState_Attack::Update(float deltaTime)
{
	
}

bool PlayerState_Attack::IsEnd()
{
	AnimInfo* animInfo = _player->GetCurrAnimation();
	if(animInfo)
		return animInfo->IsEnd;
	return true;
}
