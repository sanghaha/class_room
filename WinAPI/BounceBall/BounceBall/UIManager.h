#pragma once

//#include "Singleton.h"

class UIBase;
class UIButton;
class UIImage;

class UIManager
{
public:
	void Init();
	void Destroy();

	void Update();
	void Render(ID2D1RenderTarget* renderTarget);
	bool OnLeftClickEvent(int32 x, int32 y);

	void DestroyAllWidget();

	UIButton* CreateButton(Vector pos, wstring key, int32 width = 0, int32 height = 0);
	UIImage* CreateImage(Vector pos, wstring key, int32 width = 0, int32 height = 0);

private:
	vector<UIBase*> _uiWidget;
};

