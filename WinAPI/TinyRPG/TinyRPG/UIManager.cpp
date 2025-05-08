#include "pch.h"
#include "UIManager.h"
#include "ResourceManager.h"
#include "Scene.h"

void UIManager::Init()
{
	_uiMsg.Init();
	_uiHud.Init();
	_uiInven.Init();
}

void UIManager::Update()
{
	_uiMsg.Update();
	_uiHud.Update();
	_uiInven.Update();
}

void UIManager::Render(ID2D1HwndRenderTarget* renderTarget)
{
	_uiMsg.Render(renderTarget);
	_uiHud.Render(renderTarget);
	_uiInven.Render(renderTarget);
}

bool UIManager::OnLeftClickEvent(int32 x, int32 y)
{
	// UI 이벤트 잡아먹었는지 여부
	bool handled = false;
	handled |= (!handled && _uiMsg.OnLeftClickEvent(x, y));
	handled |= (!handled && _uiHud.OnLeftClickEvent(x, y));
	handled |= (!handled && _uiInven.OnLeftClickEvent(x, y));
	return handled;
}

void UIManager::ShowGameOver()
{
	_uiMsg.Open();
	_uiMsg.SetText(L"Game Over!");
}

void UIManager::ToggleVisibleInventory()
{
	if (_uiInven.IsOpen())
		_uiInven.Close();
	else
		_uiInven.Open();
}

