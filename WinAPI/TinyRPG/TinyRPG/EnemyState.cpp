#include "pch.h"
#include "EnemyState.h"
#include "Enemy.h"
#include "Game.h"
#include "GameScene.h"
#include "Player.h"
#include "MonsterData.h"
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
	_idleTime = (float)RandRange(1, 2); // 1~2초 사이
}

void EnemyState_Idle::Update(float deltaTime)
{
	if (_enemy->GetPosCell() == _enemy->GetSpawnedCell())
	{
		_idleTime -= deltaTime;

		// 랜덤한 시간값으로 Move State로 전환한다.
		if (_idleTime <= 0)
		{
			_enemy->ChangeState(EnemyStateType::ES_PATROL);
		}
	}
	else
	{
		// 스폰 위치와 너무 멀어지면 쫓기 중단
		int32 deltaCount = _enemy->GetPosCell().DeltaLength(_enemy->GetSpawnedCell());
		if (deltaCount > _chaseCount)
		{
			_enemy->ChangeState(EnemyStateType::ES_RETURN);
			return;
		}

		// 플레이어가 근처에 있는지 확인
		Player* player = Game::GetGameScene()->GetPlayer();
		if (player)
		{
			int32 deltaCount = player->GetPosCell().DeltaLength(_enemy->GetPosCell());
			if (deltaCount == 1)
			{
				// 바로 옆에 있다. 플레이어를 향해 방향 회전후 공격!
				_enemy->TurnToPlayerDir(player);
				_enemy->ChangeState(EnemyStateType::ES_ATTACK);
			}
			else if (deltaCount < _chaseCount)
			{
				_enemy->ChangeState(EnemyStateType::ES_CHASE);
			}
			else
			{
				_enemy->ChangeState(EnemyStateType::ES_RETURN);
			}
		}
	}
}

/// <summary>
/// 
/// </summary>
EnemyState_Patrol::EnemyState_Patrol(Enemy* enemy) : Super(enemy)
{
}

EnemyState_Patrol::~EnemyState_Patrol()
{
}

AnimType EnemyState_Patrol::GetAnimType()
{
	return AnimType::A_MOVE;
}

void EnemyState_Patrol::Enter()
{
	//근처 랜덤한 좌표
	_startCell = _enemy->GetSpawnedCell();
	_enemy->SetMoveSpeed((float)(_enemy->GetData()->_walkSpeed));

	// 25% 확률로 좌우 판단
	int32 rand = RandRange(1, 100);
	_dirX = 0;
	_dirY = 0;
	if (rand <= 25)
	{
		_dirX = -1;
	}
	else if (rand <= 50)
	{
		_dirX = 1;
	}
	else if (rand <= 75)
	{
		_dirY = -1;
	}
	else
	{
		_dirY = 1;
	}

	_enemy->Move(_dirX, _dirY);
}

void EnemyState_Patrol::Update(float deltaTime)
{
	Vector prevPos = _enemy->GetPos();
	_enemy->AddPosDelta(deltaTime);

	// 이동 완료했는지 확인
	if (prevPos == _enemy->GetPos())
	{
		// 똑똑한 녀석이라면, 이동중에 플레이어 감지.
		Player* player = Game::GetGameScene()->GetPlayer();
		if (player)
		{
			int32 deltaCount = player->GetPosCell().DeltaLength(_enemy->GetPosCell());
			if (deltaCount < _chaseCount)
			{
				_enemy->ChangeState(EnemyStateType::ES_CHASE);
				return;
			}
		}

		// 목표 지점에 도착했으면 반대 반향으로 이동
		int32 delta = _startCell.DeltaLength(_enemy->GetPosCell());
		if (delta >= _range)
		{
			_dirX *= -1;
			_dirY *= -1;
		}

		if (false == _enemy->Move(_dirX, _dirY))
		{
			_dirX *= -1;
			_dirY *= -1;
		}
	}
}

bool EnemyState_Patrol::IsEnd()
{
	// 패트롤은 무한 반복이다.
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
	_enemy->ResetAnimation(AnimType::A_ATTACK);

	_attackTime = 0.3f;
}

void EnemyState_Attack::Update(float deltaTime)
{
	if (_attackTime != 0)
	{
		_attackTime -= deltaTime;

		if (_attackTime < 0)
		{
			_attackTime = 0;

			// 플레이어 피해
			Cell posCell = _enemy->GetPosCell();
			posCell = posCell.NextCell(_enemy->GetCurrDir());

			const GridInfo& gridInfo = Game::GetScene()->GetGridInfo(posCell);
			for (auto iter : gridInfo._actors)
			{
				Player* creature = dynamic_cast<Player*>(iter);
				if (nullptr == creature)
					continue;

				creature->TakeDamage(_enemy->GetAttack());
			}
		}
	}
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
	_currPathCount = 0;
	calcChasePath();
	
	_enemy->SetMoveSpeed((float)(_enemy->GetData()->_runSpeed));
}

void EnemyState_Chase::Update(float deltaTime)
{
	Player* player = Game::GetGameScene()->GetPlayer();
	if (nullptr == player)
		return;

	if (_path.size() == 0)
		return;

	_enemy->AddPosDelta(deltaTime);

	// 이동 완료했는지 확인
	if (_path[0].ConvertToPos() == _enemy->GetPos())
	{
		// 해당 path로 이동이 완료되었다. 플레이어 위치가 변경되었으면 길을 한번더 찾는다.
		calcChasePath();
	}
}

bool EnemyState_Chase::IsEnd()
{
	// 스폰 위치와 너무 멀어지면 쫓기 중단
	int32 deltaCount = _enemy->GetPosCell().DeltaLength(_enemy->GetSpawnedCell());
	if (deltaCount > _chaseCount)
	{
		return true;
	}

	Player* player = Game::GetGameScene()->GetPlayer();
	if (player)
	{
		// 플레이어와 너무 멀어지면 쫓기 중단
		int32 deltaCount = player->GetPosCell().DeltaLength(_enemy->GetPosCell());
		if (deltaCount > _chaseCount)
		{
			return true;
		}
	}

	if (_destCell.ConvertToPos() == _enemy->GetPos())
		return true;

	if (_path.size() == 0)
		return true;

	return false;
}

void EnemyState_Chase::calcChasePath()
{
	// 너무 자주 찾으면 성능 문제가 있으니 조절
	if (_currPathCount == 0|| _currPathCount == _checkPathCount)
	{
		// path를 찾아서 path대로 이동한다.
		Cell start = _enemy->GetPosCell();
		Cell end = Game::GetGameScene()->GetPlayer()->GetPosCell();
		Game::GetGameScene()->FindPath(start, end, _path, _chaseCount);

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
		int32 xDir = _path[0].index_X - _enemy->GetPosCell().index_X;
		int32 yDir = _path[0].index_Y - _enemy->GetPosCell().index_Y;
		_enemy->Move(xDir, yDir);
	}
}

EnemyState_Return::EnemyState_Return(Enemy* enemy) : Super(enemy)
{
}

EnemyState_Return::~EnemyState_Return()
{
}

AnimType EnemyState_Return::GetAnimType()
{
	return AnimType::A_MOVE;
}

void EnemyState_Return::Enter()
{
	_enemy->SetMoveSpeed((float)(_enemy->GetData()->_runSpeed));

	// path를 찾아서 path대로 이동한다.
	Cell start = _enemy->GetPosCell();
	Cell end = _enemy->GetSpawnedCell();
	Game::GetGameScene()->FindPath(start, end, _path, 10);

	// 길을 찾았다
	if (_path.size() != 0)
	{
		int32 xDir = _path[0].index_X - _enemy->GetPosCell().index_X;
		int32 yDir = _path[0].index_Y - _enemy->GetPosCell().index_Y;
		_enemy->Move(xDir, yDir);
	}
}

void EnemyState_Return::Update(float deltaTime)
{
	if (_path.size() == 0)
		return;

	_enemy->AddPosDelta(deltaTime);

	// 이동 완료했는지 확인
	if (_path[0].ConvertToPos() == _enemy->GetPos())
	{
		// 해당 path로 이동이 완료되었다.
		_path.erase(_path.begin());

		if (_path.size() != 0)
		{
			int32 xDir = _path[0].index_X - _enemy->GetPosCell().index_X;
			int32 yDir = _path[0].index_Y - _enemy->GetPosCell().index_Y;
			_enemy->Move(xDir, yDir);
		}
	}
}

bool EnemyState_Return::IsEnd()
{
	if (_enemy->GetPosCell() == _enemy->GetSpawnedCell())
		return true;

	return false;
}
