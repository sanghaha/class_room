#pragma once

#include "Singleton.h"
#include "UIMessage.h"
#include "UIHud.h"
#include "UIInventory.h"

class BitmapTexture;
class UIManager : public Singleton<UIManager>
{
public:
	void Init();
	void Update();
	void Render(ID2D1HwndRenderTarget* renderTarget);

	void ShowGameOver();
	void ToggleVisibleInventory();

private:
	UIMessage			_uiMsg;
	UIHud				_uiHud;
	UIInventory			_uiInven;
};

