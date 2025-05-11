#include "pch.h"
#include "Block.h"
#include "Sprite.h"
#include "ResourceManager.h"

Block::Block(Vector pos) : Super(pos)
{
}

Block::~Block()
{
    _sprite = nullptr;
}

void Block::Init()
{
    Super::Init();

    _sprite = CreateSpriteComponent("Block_0", BLOCK_SIZE, BLOCK_SIZE);
}

void Block::Update(float deltaTime)
{
    Super::Update(deltaTime);
}

void Block::Render(ID2D1RenderTarget* renderTarget)
{
    Super::Render(renderTarget);
}

RenderLayer Block::GetRenderLayer()
{
    return RenderLayer::RL_Object;
}

Rect* Block::GetCollisionRect()
{
    _collision.left = GetPos().x - BLOCK_SIZE * 0.52f;
    _collision.right = GetPos().x + BLOCK_SIZE * 0.52f;
    _collision.top = GetPos().y - BLOCK_SIZE * 0.5f;
    _collision.bottom = GetPos().y + BLOCK_SIZE * 0.5f;

    return &_collision;
}