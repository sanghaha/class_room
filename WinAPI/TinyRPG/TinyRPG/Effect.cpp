#include "pch.h"
#include "Effect.h"
#include "ResourceManager.h"
#include "Game.h"
#include "GameScene.h"

EffectExplosion::EffectExplosion(Vector pos) : Super(pos)
{
}

EffectExplosion::~EffectExplosion()
{
}

void EffectExplosion::Init()
{
    Super::Init();

    _animInfo = AnimInfo(0, 0, 9, 1, false, 0.6f);

    Sprite* sprite = ResourceManager::GetInstance()->GetSprite(L"Explosion");
    _renderer.SetSprite(sprite);
    _renderer.SetAnimInfo(&_animInfo);
}

void EffectExplosion::Update(float deltaTime)
{
    _renderer.Update(deltaTime);

    if (_animInfo.IsEnd)
    {
        Game::GetGameScene()->ReserveRemove(this);
    }
}

void EffectExplosion::Render(HDC hdc)
{
    _renderer.Render(hdc, GetPos());
}

RenderLayer EffectExplosion::GetRenderLayer()
{
    return RenderLayer::RL_Effect;
}

ColliderCircle* EffectExplosion::GetCollider()
{
    return nullptr;
}
