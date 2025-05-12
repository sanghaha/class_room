#pragma once

class UIBase
{
public:
	UIBase(Vector pos);
	virtual ~UIBase();

	virtual void Init() {}
	virtual void Update() {}
	virtual void Render(ID2D1RenderTarget* renderTarget);
	virtual void Open() { _isOpen = true; }
	virtual void Close() { _isOpen = false; }
	virtual bool OnLeftClickEvent(int32 x, int32 y) { return false; }
	
	bool IsOpen() const { return _isOpen; }
	void SetOpen(bool open) { _isOpen = open; }

protected:
	bool _isOpen = true;
	Vector _pos = {};
};

