#pragma once
#include "UIBase.h"
#include "Sprite.h"

class UIHud : public UIBase
{
public:
	UIHud();
	virtual ~UIHud();

	void Init();
	void Render(ID2D1RenderTarget* renderTarget);

private:
	class Sliced3Texture*	_hpBar = nullptr;
	class Sliced3Texture*	_hpValue = nullptr;
	class Sprite*			_attackIcon = nullptr;
	class NumberSprite*		_attackValue = nullptr;
};

