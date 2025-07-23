#include "pch.h"
#include "Block.h"
#include "ResourceManager.h"

Block::Block(Vector pos) : Super(pos)
{
}

Block::~Block()
{
}

void Block::Init()
{
    Super::Init();

    CreateSpriteComponent("Block_0", BLOCK_SIZE, BLOCK_SIZE);
	CreateRectCollider(BLOCK_SIZE, BLOCK_SIZE);
}

void Block::Update(float deltaTime)
{
    Super::Update(deltaTime);
}

void Block::Render(HDC renderTarget)
{
    Super::Render(renderTarget);
}

RenderLayer Block::GetRenderLayer()
{
    return RenderLayer::RL_Object;
}