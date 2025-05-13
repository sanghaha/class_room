#include "pch.h"
#include "UIManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "UIBase.h"


void UIManager::Init()
{
}

void UIManager::Destroy()
{
	for (auto iter : _panel)
	{
		iter->DestroyAllWidget();
		SAFE_DELETE(iter);
	}
	_panel.clear();
}

void UIManager::Update(float deltaTime)
{
	for (auto iter : _panel)
	{
		if (iter->IsOpen())
		{
			iter->Update(deltaTime);
		}
	}
}

void UIManager::Render(ID2D1RenderTarget* renderTarget)
{
	for (auto iter : _panel)
	{
		if (iter->IsOpen())
		{
			iter->Render(renderTarget);
		}
	}
}

bool UIManager::OnLeftClickEvent(int32 x, int32 y)
{
	// UI �̺�Ʈ ��ƸԾ����� ����
	for (auto iter : _panel)
	{
		if (iter->IsOpen())
		{
			if (iter->OnLeftClickEvent(x, y))
			{
				return true;
			}
		}
	}
	return false;
}

void UIManager::AddPanel(UIBase* panel)
{
	_panel.emplace_back(panel);
}


