#include "pch.h"
#include "Effect.h"
#include "ResourceManager.h"
#include "Game.h"
#include "GameScene.h"

Effect::Effect(Vector pos, string spriteName) : Super(pos)
{
    // Sprite »ý¼º
    _sprite = CreateSpriteComponent(spriteName);
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
        Game::GetGameScene()->ReserveRemove(this);
    }
}

void Effect::Render(ID2D1RenderTarget* renderTarget)
{
    Super::Render(renderTarget);
}

RenderLayer Effect::GetRenderLayer()
{
    return RenderLayer::RL_Effect;
}