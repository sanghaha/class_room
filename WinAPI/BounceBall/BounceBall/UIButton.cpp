#include "pch.h"
#include "UIButton.h"
#include "InputManager.h"
#include "Texture.h"


UIButton::UIButton(Vector pos, wstring key, int32 width, int32 height) : Super(pos)
{
	_texture = new Texture(key, width, height);
}

UIButton::~UIButton()
{
	SAFE_DELETE(_texture);
}

void UIButton::Update()
{
	if (_texture == nullptr)
		return;

	if (InputManager::GetInstance()->GetButtonDown(KeyType::LeftMouse))
	{
		RECT rect;
		rect.left = _pos.x;
		rect.right = _pos.x + _texture->GetSize().Width;
		rect.top = _pos.y;
		rect.bottom = _pos.y + _texture->GetSize().Height;

		POINT mousePos = InputManager::GetInstance()->GetMousePos();
		if (IsInPoint(rect, mousePos))
		{
			if (_onClickedEvent)
				_onClickedEvent();
		}
	}
}

void UIButton::Render(ID2D1RenderTarget* renderTarget)
{
	if (_texture)
	{
		_texture->Render(renderTarget, _pos);
	}
}
