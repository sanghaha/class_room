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

void PlayerState_Idle::Enter()
{
	if (nullptr == _player)
		return;

	int32 xDir = InputManager::GetInstance()->GetMoveDirX();
	int32 yDir = InputManager::GetInstance()->GetMoveDirY();

	AnimInfo* animInfo = nullptr;
	if (xDir != 0 || yDir != 0)
		animInfo = _player->GetAnimation(PlayerAnimType::PA_MOVE);
	else
		animInfo = _player->GetAnimation(PlayerAnimType::PA_IDLE);

	_player->ChangeAnimation(animInfo);
}

void PlayerState_Idle::Update(float deltaTime)
{
	if (nullptr == _player)
		return;

	int32 xDir = InputManager::GetInstance()->GetMoveDirX();
	int32 yDir = InputManager::GetInstance()->GetMoveDirY();
	if (xDir != 0 || yDir != 0)
	{
		_player->Move(xDir, yDir);
	}
	else if (InputManager::GetInstance()->GetButtonPressed(KeyType::SpaceBar))
	{
		// Idle 상태에서만 공격 가능. 이동하면서 공격 불가능.
		_player->ChangeState(PlayerStateType::S_ATTACK);
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

void PlayerState_Move::Enter()
{
	if (nullptr == _player)
		return;

	AnimInfo* animInfo = _player->GetAnimation(PlayerAnimType::PA_MOVE);
	_player->ChangeAnimation(animInfo);

	// dest 좌표값 계산
	Cell destCell = _player->GetPosCell();
	_destPos = destCell.ConvertToPos();

	// 이전 위치 초기화
	_prevPos = _player->GetPos();

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

	// _prevPos와 currentPos 사이에 _destPos가 있는지 확인
	Vector toPrev = _prevPos - _destPos;
	toPrev.Normalize();
	Vector toCurrent = currentPos - _destPos;
	toCurrent.Normalize();

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

void PlayerState_Attack::Enter()
{
	if (nullptr == _player)
		return;

	_animType = PlayerAnimType::PA_ATTACK_SIDE;
	
	// 플레이어의 마지막 이동 방향에 맞는 애니메이션 재생
	if (_player->GetDirY() > 0)
	{
		_animType = PlayerAnimType::PA_ATTACK_DOWN;
	}
	else if (_player->GetDirY() < 0)
	{
		_animType = PlayerAnimType::PA_ATTACK_UP;
	}

	AnimInfo* animInfo = _player->GetAnimation(_animType);
	if (animInfo)
		animInfo->Reset();
	_player->ChangeAnimation(animInfo);
}

void PlayerState_Attack::Update(float deltaTime)
{
	
}

bool PlayerState_Attack::IsEnd()
{
	AnimInfo* animInfo = _player->GetAnimation(_animType);
	if(animInfo)
		return animInfo->IsEnd;
	return true;
}
