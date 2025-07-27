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
			auto checkEnableChase = [=]()
				{
					for (int32 i = 0; i < DirType::DIR_MAX; ++i)
					{
						if (Game::GetGameScene()->CanMove(player->GetPosCell().NextCell((DirType)i)))
						{
							return true;
						}
					}
					return false;
				};


			int32 deltaCount = player->GetPosCell().DeltaLength(_enemy->GetPosCell());
			if (deltaCount == 1)
			{
				// 바로 옆에 있다. 플레이어를 향해 방향 회전후 공격!
				_enemy->TurnToPlayerDir(player);
				_enemy->ChangeState(EnemyStateType::ES_ATTACK);
			}
			else if (deltaCount < _chaseCount && checkEnableChase())
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
	// 스폰 지점에서 범위 2정도 왔다갔다 한다.
	_startCell = _enemy->GetSpawnedCell();
	_enemy->SetMoveSpeed((float)(_enemy->GetData()->_walkSpeed));

	// 상하좌우로 갈수있는곳이 있는지 판단.
	Cell nextCell[4] = {
		_startCell.NextCell(DirType::DIR_LEFT, _range),
		_startCell.NextCell(DirType::DIR_RIGHT, _range),
		_startCell.NextCell(DirType::DIR_UP, _range),
		_startCell.NextCell(DirType::DIR_DOWN, _range)
	};

	vector<Cell> enableCell;
	for (int32 i = 0; i < 4; ++i)
	{
		if (Game::GetGameScene()->CanMove(nextCell[i]))
		{
			enableCell.push_back(nextCell[i]);
		}
	}
	
	if (enableCell.empty())
	{
		// 갈수있는 곳이 없다면, 그냥 Idle 상태로 전환한다.
		_enemy->ChangeState(EnemyStateType::ES_IDLE);
		return;
	}
	
	int32 rand = RandRange(0, enableCell.size()-1);

	// 목표 지점은 랜덤하게 정한다.
	_endCell = enableCell[rand];
	_dirX = clamp(_endCell.index_X - _startCell.index_X, -1, 1);
	_dirY = clamp(_endCell.index_Y - _startCell.index_Y, -1, 1);

	Cell next(_startCell.index_X + _dirX, _startCell.index_Y + _dirY);
	_enemy->Move(next);
}

void EnemyState_Patrol::Update(float deltaTime)
{
	Vector prevPos = _enemy->GetPos();
	_enemy->AddPosDelta(deltaTime);

	// 이동 완료했는지 확인
	if (_enemy->IsCompleteMove())
	{
		// 똑똑한 녀석이라면, 이동중에 플레이어 감지.
		Player* player = Game::GetGameScene()->GetPlayer();
		if (player)
		{
			// 플레이어가 chagse 범위내 일경우 Chase 스테이트로 전환
			int32 deltaCount = player->GetPosCell().DeltaLength(_enemy->GetPosCell());
			if (deltaCount < _chaseCount)
			{
				_enemy->ChangeState(EnemyStateType::ES_CHASE);
				return;
			}
		}

		// 목표 지점에 도착했으면 반대 반향으로 이동
		int32 delta = _endCell.DeltaLength(_enemy->GetPosCell());
		if (delta == 0)
		{
			swap(_startCell, _endCell);

			_dirX *= -1;
			_dirY *= -1;
		}

		Cell next(_enemy->GetPosCell().index_X + _dirX, _enemy->GetPosCell().index_Y + _dirY);
		_enemy->Move(next);
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
			_enemy->Attack();
		}
	}
}

bool EnemyState_Attack::IsEnd()
{
	return _enemy->IsEndAnimation();
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
	PrintLog(std::format(L"(1)Enter::Chase{0} :{1},{2}", (int64)this, _enemy->GetPosCell().index_X, _enemy->GetPosCell().index_Y));

	Super::Enter();
	_enemy->SetMoveSpeed((float)(_enemy->GetData()->_runSpeed));

	Cell dest = getPathDest();
	_pathFind.Start(_enemy, _enemy->GetPosCell(), dest, 3, std::bind(&EnemyState_Chase::getPathDest, this));
}

void EnemyState_Chase::Update(float deltaTime)
{
	Super::Update(deltaTime);
	_pathFind.Update(deltaTime);
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

	if (_pathFind.IsComplete())
		return true;

	return false;
}

Cell EnemyState_Chase::getPathDest()
{
	Player* player = Game::GetGameScene()->GetPlayer();
	if (player)
		return player->GetPosCell();
	return _enemy->GetSpawnedCell();
}

/// <summary>
/// 
/// </summary>
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
	Super::Enter();
	_enemy->SetMoveSpeed((float)(_enemy->GetData()->_runSpeed));
	Cell dest = getPathDest();
	_pathFind.Start(_enemy, _enemy->GetPosCell(), dest, 3, std::bind(&EnemyState_Return::getPathDest, this));
}

void EnemyState_Return::Update(float deltaTime)
{
	Super::Update(deltaTime);
	_pathFind.Update(deltaTime);
}

bool EnemyState_Return::IsEnd()
{
	if (_enemy->GetPosCell() == _enemy->GetSpawnedCell())
		return true;

	if (_pathFind.IsComplete())
		return true;

	return false;
}

Cell EnemyState_Return::getPathDest()
{
	return _enemy->GetSpawnedCell();
}


