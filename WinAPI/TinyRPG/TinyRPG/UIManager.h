#pragma once

#include "Singleton.h"
#include "UIMessage.h"
#include "UIHud.h"

class BitmapTexture;
class UIManager : public Singleton<UIManager>
{
public:
	void Init();
	void Render(ID2D1HwndRenderTarget* renderTarget);

	void ShowGameOver();

private:
	UIMessage	_uiMsg;
	UIHud		_uiHud;
};

