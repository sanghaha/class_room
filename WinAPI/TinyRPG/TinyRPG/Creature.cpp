#include "pch.h"
#include "Creature.h"
#include "Sprite.h"
#include "Game.h"

Creature::Creature(Vector pos) : Super(pos)
{
}

Creature::~Creature()
{
}


void Creature::Init()
{
	Super::Init();
}

void Creature::Update(float deltaTime)
{
	_renderer.Update(deltaTime);
	_stateMachine.Update(deltaTime);

	_collider.Update();
}

void Creature::Render(HDC hdc)
{
	_renderer.Render(hdc, GetPos(), _dirX);

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
	}

	return true;
}

void Creature::ChangeAnimation(AnimInfo* animInfo)
{
	_renderer.SetAnimInfo(animInfo);
}

void Creature::ChangeState(int32 stateType)
{
	_stateMachine.ReserveNextState(stateType);
}
