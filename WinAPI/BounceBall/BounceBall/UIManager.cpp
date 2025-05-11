#include "pch.h"
#include "UIManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "UIButton.h"
#include "UIImage.h"


void UIManager::Destroy()
{
	DestroyAllWidget();
}

void UIManager::Init()
{

}

void UIManager::Update()
{
	for (auto iter : _uiWidget)
	{
		if (iter->IsOpen())
		{
			iter->Update();
		}
	}
}

void UIManager::Render(ID2D1RenderTarget* renderTarget)
{
	for (auto iter : _uiWidget)
	{
		if (iter->IsOpen())
		{
			iter->Render(renderTarget);
		}
	}
}

bool UIManager::OnLeftClickEvent(int32 x, int32 y)
{
	// UI 이벤트 잡아먹었는지 여부
	bool handled = false;
	return handled;
}

void UIManager::DestroyAllWidget()
{
	for (auto iter : _uiWidget)
	{
		SAFE_DELETE(iter);
	}
	_uiWidget.clear();
}

UIButton* UIManager::CreateButton(Vector pos, wstring key, int32 width, int32 height)
{
	UIButton* button = new UIButton(pos, key);
	_uiWidget.emplace_back(button);
	return button;
}

UIImage* UIManager::CreateImage(Vector pos, wstring key, int32 width, int32 height)
{
	UIImage* image = new UIImage(pos, key);
	_uiWidget.emplace_back(image);
	return image;
}




