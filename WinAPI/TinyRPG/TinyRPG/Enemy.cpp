#include "pch.h"
#include "Enemy.h"
#include "EnemyState.h"
#include "MonsterData.h"
#include "Player.h"
#include "DataManager.h"
#include "Game.h"
#include "GameScene.h"
#include "Projectile.h"

Enemy::Enemy(const MonsterData* data, Vector pos) : Super(pos)
{
	_hp = data->_hp;
	_maxHp = _hp;
	_spawnedCell = GetPosCell();
	_data = data;
	_attack = data->_attack;
	SetMoveSpeed((float)data->_walkSpeed);
}

Enemy::~Enemy()
{
}

void Enemy::Init()
{
	Super::Init();

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

void Enemy::Render(ID2D1RenderTarget* renderTarget)
{
	if (IsCulling())
		return;
	Super::Render(renderTarget);
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

void Enemy::ResetAnimation(AnimType type)
{
	for (int32 i = 0; i < DirType::DIR_MAX; ++i)
	{
		_animInfo[type][i].Reset();
	}
}

bool Enemy::CanAttackToTarget(Creature* target)
{
	Player* player = dynamic_cast<Player*>(target);
	return (player != nullptr);
}

void Enemy::OnBeginOverlapActor(Actor* other)
{
	// 화살과 중첩되면 데미지 처리
	Projectile* arrow = dynamic_cast<Projectile*>(other);
	if (arrow)
	{
		TakeDamage(arrow->GetAttack());
		arrow->Destory();
	}
}

void Enemy::OnDead()
{
	// 랜덤 아이템 생성
	int32 itemId = DataManager::GetInstance()->GetRandomItemId();
	Game::GetGameScene()->CreateDropItem(GetPos(), itemId);
}

AnimInfo* Enemy::calcDirAnim(AnimType type)
{
	return &_animInfo[type][_currDir];
}
