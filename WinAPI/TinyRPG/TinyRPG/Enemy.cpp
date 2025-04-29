#include "pch.h"
#include "Enemy.h"
#include "EnemyState.h"
#include "MonsterData.h"
#include "Player.h"

Enemy::Enemy(const MonsterData* data, Vector pos) : Super(pos)
{
	_hp = data->_hp;
	_spawnedCell = GetPosCell();
	SetMoveSpeed((float)data->_speed);
}

Enemy::~Enemy()
{
}

void Enemy::Init()
{
	Super::Init();

	_collider.SetEnterCollisionCallback(
		[this](ColliderCircle* src, ColliderCircle* other) {
			this->OnEnterCollision(src, other);
		}
	);

	_collider.SetExitCollisionCallback(
		[this](ColliderCircle* src, ColliderCircle* other) {
			this->OnExitCollision(src, other);
		}
	);

	_collider.SetOverlapCollisionCallback(
		[this](ColliderCircle* src, ColliderCircle* other) {
			this->OnOverlapCollision(src, other);
		}
	);


	// 애니메이션 정보
	{
		AnimInfo info = AnimInfo(0, 0, 7, 1, true, 0.7f);
		for (int32 i = 0; i < DirType::DIR_MAX; ++i)
		{
			_animInfo[AnimType::A_IDLE][i] = info;
		}
		_animInfo[AnimType::A_IDLE][DirType::DIR_LEFT].FlipX = -1;
	}
	{
		AnimInfo info = AnimInfo(0, 1, 6, 1, true, 0.6f);
		for (int32 i = 0; i < DirType::DIR_MAX; ++i)
		{
			_animInfo[AnimType::A_MOVE][i] = info;
		}
		_animInfo[AnimType::A_MOVE][DirType::DIR_LEFT].FlipX = -1;
	}
	{
		_animInfo[AnimType::A_ATTACK][DirType::DIR_RIGHT] = AnimInfo(0, 2, 6, 1, false, 0.6f);
		_animInfo[AnimType::A_ATTACK][DirType::DIR_LEFT] = AnimInfo(0, 2, 6, 1, false, 0.6f, -1);
		_animInfo[AnimType::A_ATTACK][DirType::DIR_DOWN] = AnimInfo(0, 3, 6, 1, false, 0.6f);
		_animInfo[AnimType::A_ATTACK][DirType::DIR_UP] = AnimInfo(0, 4, 6, 1, false, 0.6f);
	}

	// state 정보
	_stateMachine.AddState(new EnemyState_Idle(this));
	_stateMachine.AddState(new EnemyState_Patrol(this));
	_stateMachine.AddState(new EnemyState_Chase(this));
	_stateMachine.AddState(new EnemyState_Attack(this));
	_stateMachine.AddState(new EnemyState_Return(this));

	_stateMachine.SetDefaultState((int32)EnemyStateType::ES_IDLE);
	_stateMachine.ReserveNextState((int32)EnemyStateType::ES_IDLE);
}

void Enemy::Update(float deltaTime)
{
	Super::Update(deltaTime);
}

void Enemy::Render(HDC hdc)
{
	Super::Render(hdc);
}

void Enemy::OnEnterCollision(ColliderCircle* src, ColliderCircle* other)
{
}

void Enemy::OnExitCollision(ColliderCircle* src, ColliderCircle* other)
{
}

void Enemy::OnOverlapCollision(ColliderCircle* src, ColliderCircle* other)
{
}

bool Enemy::Move(int32 dirX, int32 dirY)
{
	bool result = Super::Move(dirX, dirY);
	return result;
}

void Enemy::TurnToPlayerDir(Player* player)
{
	if (player->GetPosCell().index_X != GetPosCell().index_X)
	{
		_currDir = player->GetPosCell().index_X > GetPosCell().index_X ? DirType::DIR_RIGHT : DirType::DIR_LEFT;
	}
	else if (player->GetPosCell().index_Y != GetPosCell().index_Y)
	{
		_currDir = player->GetPosCell().index_Y > GetPosCell().index_Y ? DirType::DIR_DOWN : DirType::DIR_UP;
	}
}
