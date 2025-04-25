#include "pch.h"
#include "Creature.h"
#include "Sprite.h"

Creature::Creature(Pos pos) : Super(pos)
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

	Size size = sprite->GetSize();
	AddPosDelta(-size.Width / 2.0f, -size.Height / 2.0f);

	// 원의 중심과 반지름 설정
	_collider.Init(this, sprite->GetSize(), GetPos());
}