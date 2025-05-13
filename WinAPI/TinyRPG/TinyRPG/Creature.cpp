#include "pch.h"
#include "Creature.h"
#include "Sprite.h"
#include "Game.h"
#include "GameScene.h"
#include "InputManager.h"

Creature::Creature(Vector pos) : Super(pos)
{
}

Creature::~Creature()
{
}


void Creature::Init()
{
	Super::Init();
	ChangeAnimation(_curAnimType);
}

void Creature::Update(float deltaTime)
{
	_stateMachine.Update(deltaTime);

	// State 에 따른 애니메이션 변환
	AnimType animType = _stateMachine.GetCurrStateAnimType();
	ChangeAnimation(animType);

	Super::Update(deltaTime);
}

void Creature::Render(ID2D1RenderTarget* renderTarget)
{
	Super::Render(renderTarget);

	if (_sprite)
	{
		_lastRenderFlipX = _sprite->GetFlip();
	}
}

bool Creature::Move(int32 dirX, int32 dirY)
{
	Cell next = GetPosCell();
	next.index_X += dirX;
	next.index_Y += dirY;

	if (false == Game::CanMove(next))
	{
		// 못감
		return false;
	}
	else
	{
		//PrintLog(std::format(L"~~~~ Move : {0},{1} -> {2},{3}",
		//	GetPosCell().index_X, GetPosCell().index_Y,
		//	next.index_X, next.index_Y));

		SetPosCell(next);

		_dirX = dirX;
		_dirY = dirY;

		if (dirX != 0)
		{
			_currDir = (dirX > 0) ? DirType::DIR_RIGHT : DirType::DIR_LEFT;
		}
		if (dirY != 0)
		{
			_currDir = (dirY > 0) ? DirType::DIR_DOWN : DirType::DIR_UP;
		}
	}

	return true;
}

void Creature::ChangeAnimation(AnimType type)
{
	if (_sprite == nullptr)
		return;

	bool lastRenderFlipX = _lastRenderFlipX;
	AnimInfo* animInfo = calcDirAnim(type);
	if (nullptr == animInfo)
		return;

	if (type != _curAnimType)
	{
		_animIndexX = animInfo->startX;
		_animIndexY = animInfo->startY;
		_sumTime = 0;
		_isEnd = false;
		_curAnimType = type;
	}
	
	// 같은 애니메이션 상태라도, 방향이 달라질수도 있다.
	//AnimInfo* curInfo = _renderer.GetAimInfo();
	//if (curInfo && animInfo != curInfo)
	//{
	//	animInfo->IndexX = curInfo->IndexX;
	//	animInfo->IndexY = curInfo->IndexY;
	//	animInfo->SumTime = curInfo->SumTime;
	//}
	//_renderer.SetAnimInfo(animInfo);

	// 아래, 위 방향은 좌우 flipX 정보를 이어받는다.
	if (_currDir == DirType::DIR_DOWN || _currDir == DirType::DIR_UP)
	{
		animInfo->flipX = lastRenderFlipX;
	}

	SetAnimInfo(*animInfo);
}

void Creature::ChangeState(int32 stateType)
{
	_stateMachine.ReserveNextState(stateType);
}

bool Creature::CanAttackToTarget(Creature* target)
{
	return false;
}

void Creature::Attack()
{
	Cell posCell = GetPosCell();
	posCell = posCell.NextCell(GetCurrDir());

	const GridInfo& gridInfo = Game::GetScene()->GetGridInfo(posCell);
	for (auto iter : gridInfo._actorsInCell)
	{
		Creature* creature = dynamic_cast<Creature*>(iter);
		if (nullptr == creature)
			continue;

		if (CanAttackToTarget(creature))
		{
			creature->TakeDamage(GetAttack());
		}
	}
}

void Creature::TakeDamage(int32 damage)
{
	_hp -= damage;
	Game::GetGameScene()->CreateExplosionEffect(GetPos());

	if (_hp <= 0)
	{
		Game::GetGameScene()->ReserveRemove(this);
		OnDead();
	}
}

void Creature::ChangeStat(StatType statType, int32 value)
{
	switch (statType)
	{
	case StatType::NoType:
		break;
	case StatType::Attack:
		_attack += value;
		break;
	case StatType::Defense:
		_defense += value;
		break;
	case StatType::HP:
		_hp = min(_hp + value, _maxHp);
		break;
	default:
		break;
	}
}

AnimInfo* Creature::calcDirAnim(AnimType type)
{
	return nullptr;
}
