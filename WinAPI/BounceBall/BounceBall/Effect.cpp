#include "pch.h"
#include "Effect.h"

Effect::Effect(Vector pos, string spriteName) : Super(pos), _spriteName(spriteName)
{
}

Effect::~Effect()
{
}

void Effect::Init()
{
	Super::Init();

	_sprite = CreateSpriteComponent(_spriteName);
}

void Effect::Update(float deltaTime)
{
	Super::Update(deltaTime);

	if(false == _loop && true == _isEnd && true == _autoDestroy)
	{
		Destroy();
	}
}
