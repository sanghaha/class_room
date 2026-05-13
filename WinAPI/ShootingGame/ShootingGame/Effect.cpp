#include "pch.h"
#include "Effect.h"
#include "Sprite.h"
#include "Scene.h"
#include "Game.h"
#include "SpriteRenderer.h"

Effect::Effect(Pos pos, wstring bitmapKey, float frameTime) : Super(pos)
{
	_spriteRenderer = CreateSpriteComponent(bitmapKey);
}

Effect::~Effect()
{
}

void Effect::Init()
{
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