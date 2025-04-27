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

	_renderer.Update(deltaTime);
	_collider.Update();
}

void Creature::Render(HDC hdc)
{
	_renderer.Render(hdc, GetPos());

	// 충돌체크를 위한 원 그리기
	_collider.Render(hdc);
}

void Creature::SetTexture(Sprite* sprite)
{
	if (sprite == nullptr)
		return;

	_renderer.SetSprite(sprite);

	// 원의 중심과 반지름 설정
	_collider.Init(this, sprite->GetSize(), GetPos());
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
			_isRightDir = (dirX > 0) ? true : false;
	}

	return true;
}

void Creature::ChangeAnimation(AnimType type)
{
	AnimInfo* animInfo = calcDirAnim(type);
	if (type != _curAnimType)
	{
		_curAnimType = type;
		_renderer.SetAnimInfo(animInfo);
	}
	else if(type == AnimType::A_IDLE || type == AnimType::A_MOVE)
	{
		// 같은 애니메이션 상태라도, 방향이 달라질수도 있다.
		AnimInfo* curInfo = _renderer.GetAimInfo();
		if (curInfo && animInfo != curInfo)
		{
			animInfo->IndexX = curInfo->IndexX;
			animInfo->IndexY = curInfo->IndexY;
			animInfo->SumTime = curInfo->SumTime;
		}
		_renderer.SetAnimInfo(animInfo);
	}
}

void Creature::ResetAnimation(AnimType type)
{
	for (int32 i = 0; i < DirType::DIR_MAX; ++i)
	{
		_animInfo[type][i].Reset();
	}
}

void Creature::ChangeState(int32 stateType)
{
	_stateMachine.ReserveNextState(stateType);
}

void Creature::TakeDamage(int32 damage)
{
	_hp -= damage;

	if (_hp <= 0)
	{
		Game::GetGameScene()->ReserveRemove(this);
	}
}

AnimInfo* Creature::calcDirAnim(AnimType type)
{
	int32 moveX = InputManager::GetInstance()->GetMoveDirX();
	if (moveX > 0)
	{
		return &_animInfo[type][DirType::DIR_RIGHT];
	}
	if (moveX < 0)
	{
		return &_animInfo[type][DirType::DIR_LEFT];
	}

	// 마지막 방향 기준
	_animInfo[type][DirType::DIR_DOWN].FlipX = _isRightDir ? 1 : -1;
	_animInfo[type][DirType::DIR_UP].FlipX = _isRightDir ? 1 : -1;

	int32 moveY = InputManager::GetInstance()->GetMoveDirY();
	if (moveY > 0)
	{
		return &_animInfo[type][DirType::DIR_DOWN];
	}
	if (moveY < 0)
	{
		return &_animInfo[type][DirType::DIR_UP];
	}

	// 키입력이 없다면, 마지막 상태값으로
	if (_isRightDir)
	{
		return &_animInfo[type][DirType::DIR_RIGHT];
	}
	
	return &_animInfo[type][DirType::DIR_LEFT];
}
