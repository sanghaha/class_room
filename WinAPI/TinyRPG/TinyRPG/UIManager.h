#pragma once

class UIBase;

class UIManager
{
public:
	void Init();
	void Destroy();
	void Update(float deltaTime);
	void Render(ID2D1RenderTarget* renderTarget);
	bool OnLeftClickEvent(int32 x, int32 y);

	void AddPanel(UIBase* panel);

private:
	vector<UIBase*>		_panel;
};

