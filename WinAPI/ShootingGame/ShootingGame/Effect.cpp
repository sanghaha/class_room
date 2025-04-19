#include "pch.h"
#include "Effect.h"
#include "Sprite.h"
#include "Scene.h"

Effect::Effect(Pos pos) : Super(pos)
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
	_renderer.Update(deltaTime);

	if (_renderer.IsEnd())
	{
		Scene::GetInstance()->ReserveRemove(this);
	}
}

void Effect::Render(HDC hdc)
{
	_renderer.Render(hdc, GetPos());
}

void Effect::SetTexture(Sprite* texture, float frameTime)
{
	_renderer.SetSprite(texture, frameTime);

	Size size = texture->GetSize();
	AddPosDelta(-size.w * 0.5f, 0);
}
