#include "pch.h"
#include "Star.h"
#include "Sprite.h"

Star::Star(Vector pos) : Super(pos)
{
}

Star::~Star()
{
	_sprite = nullptr;
}

void Star::Init()
{
	Super::Init();

	_sprite = CreateSpriteComponent("Star_0", BLOCK_SIZE, BLOCK_SIZE);
}

void Star::Update(float deltaTime)
{
	Super::Update(deltaTime);
}

void Star::Render(ID2D1RenderTarget* renderTarget)
{
	Super::Render(renderTarget);
}

RenderLayer Star::GetRenderLayer()
{
	return RenderLayer::RL_Object;
}

Rect* Star::GetCollisionRect()
{
	_collision.left = GetPos().x - BLOCK_SIZE * 0.52f;
	_collision.right = GetPos().x + BLOCK_SIZE * 0.52f;
	_collision.top = GetPos().y - BLOCK_SIZE * 0.5f;
	_collision.bottom = GetPos().y + BLOCK_SIZE * 0.5f;

	return &_collision;
}