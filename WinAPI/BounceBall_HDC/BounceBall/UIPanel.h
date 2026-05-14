#pragma once
#include "UIBase.h"

class UIButton;
class UIImage;

class UIPanel : public UIBase
{
	using Super = UIBase;
public:
	UIPanel(Vector pos);
	virtual ~UIPanel();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC renderTarget) override;

	void Show() { Open(); }
	void Hide() { Close(); }

	UIButton* CreateButton(Vector pos, wstring key, int32 width = 0, int32 height = 0);
	UIImage* CreateImage(Vector pos, wstring key, int32 width = 0, int32 height = 0);

private:
	vector<UIBase*> _children;
};
