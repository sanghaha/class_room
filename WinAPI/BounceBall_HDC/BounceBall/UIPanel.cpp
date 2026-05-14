#include "pch.h"
#include "UIPanel.h"
#include "UIButton.h"
#include "UIImage.h"

UIPanel::UIPanel(Vector pos) : Super(pos)
{
}

UIPanel::~UIPanel()
{
	for (auto iter : _children)
	{
		SAFE_DELETE(iter);
	}
	_children.clear();
}

void UIPanel::Init()
{
}

void UIPanel::Update()
{
	if (!IsOpen())
		return;

	for (auto iter : _children)
	{
		if (iter->IsOpen())
		{
			iter->Update();
		}
	}
}

void UIPanel::Render(HDC renderTarget)
{
	if (!IsOpen())
		return;

	for (auto iter : _children)
	{
		if (iter->IsOpen())
		{
			iter->Render(renderTarget);
		}
	}
}

UIButton* UIPanel::CreateButton(Vector pos, wstring key, int32 width, int32 height)
{
	UIButton* button = new UIButton(pos, key, width, height);
	_children.emplace_back(button);
	return button;
}

UIImage* UIPanel::CreateImage(Vector pos, wstring key, int32 width, int32 height)
{
	UIImage* image = new UIImage(pos, key, width, height);
	_children.emplace_back(image);
	return image;
}
