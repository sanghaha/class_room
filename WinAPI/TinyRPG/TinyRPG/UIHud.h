#pragma once
#include "UIBase.h"
#include "Sprite.h"

class UIHud : public UIBase
{
	using Super = UIBase;
public:
	UIHud();
	virtual ~UIHud();

	void Init() override;
	void Update(float deltaTime) override;
	void Render(ID2D1RenderTarget* renderTarget) override;

private:
	class UISliced3*	_hpValue = nullptr;
	class UINumber*		_attackValue = nullptr;
};

