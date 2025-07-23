#include "pch.h"
#include "Star.h"
#include "SpriteRenderer.h"

Star::Star(Vector pos) : Super(pos)
{
}

Star::~Star()
{
}

void Star::Init()
{
	Super::Init();

	SpriteRenderer* renderer = CreateSpriteComponent("Star_0", STAR_SIZE, STAR_SIZE);
	CreateRectCollider(STAR_SIZE, STAR_SIZE);
}

void Star::Update(float deltaTime)
{
	Super::Update(deltaTime);
}

void Star::Render(HDC renderTarget)
{
	Super::Render(renderTarget);
}

RenderLayer Star::GetRenderLayer()
{
	return RenderLayer::RL_Object;
}