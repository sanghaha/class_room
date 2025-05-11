#include "pch.h"
#include "Background.h"

Background::Background(Vector pos) : Super(pos)
{
}

Background::~Background()
{
}

void Background::Init()
{
	Super::Init();

	CreateTextureComponent(L"GameBG");
}

void Background::Update(float deltaTime)
{
	Super::Update(deltaTime);
}

void Background::Render(ID2D1RenderTarget* renderTarget)
{
	Super::Render(renderTarget);
}

RenderLayer Background::GetRenderLayer()
{
	return RenderLayer::RL_Background;
}
