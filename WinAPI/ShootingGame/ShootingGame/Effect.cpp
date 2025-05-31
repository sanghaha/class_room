#include "pch.h"
#include "Effect.h"
#include "Sprite.h"
#include "Scene.h"
#include "Game.h"

Effect::Effect(Pos pos, wstring bitmapKey, float frameTime) : Super(pos, bitmapKey, frameTime)
{

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

	if (_isEnd)
	{
		Destroy();
	}
}

void Effect::Render(HDC hdc)
{
	Super::Render(hdc);
}