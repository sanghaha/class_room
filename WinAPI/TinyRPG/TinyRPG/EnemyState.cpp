#include "pch.h"
#include "EnemyState.h"
#include "Enemy.h"
#include "Game.h"
#include "GameScene.h"
#include "Player.h"
/// <summary>
/// 
/// </summary>
EnemyState_Idle::EnemyState_Idle(Enemy* enemy) : Super(enemy)
{
}

EnemyState_Idle::~EnemyState_Idle()
{
}

AnimType EnemyState_Idle::GetAnimType()
{
	return AnimType::A_IDLE;
}

void EnemyState_Idle::Enter()
{
}

void EnemyState_Idle::Update(float deltaTime)
{
	// 랜덤한 시간값으로 Move State로 전환한다.
	// 플레이어가 근처에 있으면 추격
}

/// <summary>
/// 
/// </summary>
EnemyState_Wanter::EnemyState_Wanter(Enemy* enemy) : Super(enemy)
{
}

EnemyState_Wanter::~EnemyState_Wanter()
{
}

AnimType EnemyState_Wanter::GetAnimType()
{
	return AnimType::A_MOVE;
}

void EnemyState_Wanter::Enter()
{
}

void EnemyState_Wanter::Update(float deltaTime)
{
	// 똑똑한 녀석이라면, 이동중에 플레이어 감지.
}

bool EnemyState_Wanter::IsEnd()
{
	// 목표지점에 도착했는지 체크
	Vector currentPos = _enemy->GetPos();
	if (_destPos.x == currentPos.x && _destPos.y == currentPos.y)
	{
		return true;
	}
	return false;
}

/// <summary>
/// 
/// </summary>
EnemyState_Attack::EnemyState_Attack(Enemy* enemy) : Super(enemy)
{
}

EnemyState_Attack::~EnemyState_Attack()
{
}

AnimType EnemyState_Attack::GetAnimType()
{
	return AnimType::A_ATTACK;
}

void EnemyState_Attack::Enter()
{

}

void EnemyState_Attack::Update(float deltaTime)
{
}

bool EnemyState_Attack::IsEnd()
{
	AnimInfo* animInfo = _enemy->GetCurrAnimation();
	if (animInfo)
		return animInfo->IsEnd;
	return true;
}

/// <summary>
/// 
/// </summary>
EnemyState_Chase::EnemyState_Chase(Enemy* enemy) : Super(enemy)
{
}

EnemyState_Chase::~EnemyState_Chase()
{
}

AnimType EnemyState_Chase::GetAnimType()
{
	return AnimType::A_MOVE;
}

void EnemyState_Chase::Enter()
{
}

void EnemyState_Chase::Update(float deltaTime)
{
}

bool EnemyState_Chase::IsEnd()
{
	return false;
}
