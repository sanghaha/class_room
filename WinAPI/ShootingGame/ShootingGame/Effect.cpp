#include "pch.h"
#include "Effect.h"
#include "Sprite.h"
#include "Scene.h"
#include "Game.h"
#include "SpriteRenderer.h"

Effect::Effect()
{
}

Effect::~Effect()
{
}

void Effect::Init(Pos pos, wstring bitmapKey, float frameTime)
{
	SetPos(pos, false);

	if (_spriteRenderer == nullptr)
	{
		_spriteRenderer = CreateSpriteComponent(bitmapKey);
	}

	Super::Init();
}

void Effect::Update(float deltaTime)
{
	Super::Update(deltaTime);

	if (false == _spriteRenderer->GetLoop() && true == _spriteRenderer->IsEnd())
	{
		Destroy();
	}
}

void Effect::Render(HDC hdc)
{
	Super::Render(hdc);
}
