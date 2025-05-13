#pragma once

class UIBase
{
public:
	UIBase();
	virtual ~UIBase();
	
	virtual void Init();
	virtual void Update(float deltaTime);
	virtual void Render(ID2D1RenderTarget* renderTarget);
	virtual void Open() { _isOpen = true; }
	virtual void Close() { _isOpen = false; }
	virtual bool OnLeftClickEvent(int32 x, int32 y) { return false; }
	bool IsOpen() const { return _isOpen; }

	void DestroyAllWidget();

	// UI
	class UIButton* CreateButton(Vector pos, string key, int32 width = 0, int32 height = 0);
	class UIImage* CreateImage(Vector pos, string key, int32 width = 0, int32 height = 0);
	class UISprite* CreateSprite(Vector pos, string key, int32 width = 0, int32 height = 0);
	class UISliced3* CreateSliced3(Vector pos, string key, int32 left, int32 right, int32 width = 0, int32 height = 0);
	class UINumber* CreateNumber(Vector pos, string key, int32 width = 0, int32 height = 0);

protected:
	bool _isOpen = false;
	vector<class UIWidget*> _uiWidget;
};

