#include "pch.h"
#include "Effect.h"
#include "SpriteRenderer.h"

Effect::Effect(Vector pos, string spriteName) : Super(pos), _spriteName(spriteName)
{
}

Effect::~Effect()
{
}

void Effect::Init()
{
	Super::Init();

	_spriteRenderer = CreateSpriteComponent(_spriteName);
}

void Effect::Update(float deltaTime)
{
	Super::Update(deltaTime);

	if(false == _spriteRenderer->GetLoop() && true == _spriteRenderer->IsEnd() && true == _autoDestroy)
	{
		Destroy();
	}
}

void Effect::Render(HDC renderTarget)
{
	Super::Render(renderTarget);
}
