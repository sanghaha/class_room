#include "pch.h"
#include "UIManager.h"
#include "ResourceManager.h"
#include "Scene.h"

void UIManager::Init()
{
	// resource 
	ResourceManager::GetInstance()->LoadSlicedTexture(L"Ribbon_Red_3Slides", L"UI/Ribbon_Red_3Slides.png", 70, 70);
	ResourceManager::GetInstance()->LoadSlicedTexture(L"Health_03", L"UI/Health_03.png", 20, 20);
	ResourceManager::GetInstance()->LoadSlicedTexture(L"Health_03_Bar01", L"UI/Health_03_Bar01.png", 20, 20);

	_uiMsg.Init();
	_uiHud.Init();
}

void UIManager::Render(ID2D1HwndRenderTarget* renderTarget)
{
	_uiMsg.Render(renderTarget);
	_uiHud.Render(renderTarget);
}

void UIManager::ShowGameOver()
{
	_uiMsg.Open();
	_uiMsg.SetText(L"Game Over!");
}
