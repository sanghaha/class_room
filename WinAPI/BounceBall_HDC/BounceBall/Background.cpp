#include "pch.h"
#include "Background.h"
#include "ImageRenderer.h"

Background::Background(Vector pos) : Super(pos)
{
}

Background::~Background()
{
}

void Background::Init()
{
	Super::Init();

	auto imageRenderer = CreateTextureComponent(L"GameBG");
	imageRenderer->SetCenterAlign(false);
}

void Background::Update(float deltaTime)
{
	Super::Update(deltaTime);
}

void Background::Render(HDC renderTarget)
{
	Super::Render(renderTarget);
}

ActorType Background::GetActorType()
{
	return ActorType::AT_BACKGROUND;
}

RenderLayer Background::GetRenderLayer()
{
	return RenderLayer::RL_Background;
}
