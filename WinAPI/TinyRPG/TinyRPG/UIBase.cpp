#include "pch.h"
#include "UIBase.h"
#include "UIButton.h"
#include "UIImage.h"

UIBase::UIBase()
{
}

UIBase::~UIBase()
{
}

void UIBase::Init()
{
}

void UIBase::Update(float deltaTime)
{
	for (auto iter : _uiWidget)
	{
		iter->Update(deltaTime);
	}
}

void UIBase::Render(ID2D1RenderTarget* renderTarget)
{
	for (auto iter : _uiWidget)
	{
		iter->Render(renderTarget);
	}
}

void UIBase::DestroyAllWidget()
{
	for (auto iter : _uiWidget)
	{
		SAFE_DELETE(iter);
	}
	_uiWidget.clear();
}

UIButton* UIBase::CreateButton(Vector pos, string key, int32 width, int32 height)
{
	UIButton* button = new UIButton(pos, key, width, height);
	_uiWidget.emplace_back(button);
    return button;
}

UIImage* UIBase::CreateImage(Vector pos, string key, int32 width, int32 height)
{
	UIImage* image = new UIImage(pos, key, width, height);
	_uiWidget.emplace_back(image);
	return image;
}

UISprite* UIBase::CreateSprite(Vector pos, string key, int32 width, int32 height)
{
	UISprite* widget = new UISprite(pos, key, width, height);
	_uiWidget.emplace_back(widget);
	return widget;
}

UISliced3* UIBase::CreateSliced3(Vector pos, string key, int32 left, int32 right, int32 width, int32 height)
{
	UISliced3* widget = new UISliced3(pos, key, width, height, left, right);
	_uiWidget.emplace_back(widget);
	return widget;
}

UINumber* UIBase::CreateNumber(Vector pos, string key, int32 width, int32 height)
{
	UINumber* widget = new UINumber(pos, key, width, height);
	_uiWidget.emplace_back(widget);
	return widget;
}
