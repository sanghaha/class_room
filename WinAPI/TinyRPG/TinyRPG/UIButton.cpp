#include "pch.h"
#include "UIButton.h"
#include "InputManager.h"
#include "Texture.h"


UIButton::UIButton(Vector pos, string key, int32 width, int32 height) : Super(pos)
{
	_texture = new Texture(key, width, height);
}

UIButton::~UIButton()
{
	SAFE_DELETE(_texture);
}

void UIButton::Update(float deltaTime)
{
	if (_visible == false)
		return;

	if (_texture == nullptr)
		return;

	Super::Update(deltaTime);

	if (InputManager::GetInstance()->GetButtonDown(KeyType::LeftMouse))
	{
		RECT rect;
		rect.left = (int32)GetPos().x;
		rect.right = (int32)GetPos().x + _texture->GetSize().Width;
		rect.top = (int32)GetPos().y;
		rect.bottom = (int32)GetPos().y + _texture->GetSize().Height;

		POINT mousePos = InputManager::GetInstance()->GetMousePos();
		if (IsInPoint(rect, mousePos))
		{
			if (_onClickedEvent)
				_onClickedEvent();
		}
	}
}

