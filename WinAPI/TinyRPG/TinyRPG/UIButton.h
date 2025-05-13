#pragma once
#include "UIWidget.h"

class Texture;

class UIButton : public UIWidget
{
	using Super = UIWidget;

public:
	UIButton(Vector pos, string key, int32 width = 0, int32 height = 0);
	virtual ~UIButton();

	void Update(float deltaTime) override;

	void SetClickEvent(std::function<void()> click) { _onClickedEvent = click; }

private:
	std::function<void()> _onClickedEvent;
	Texture* _texture = nullptr;
};

