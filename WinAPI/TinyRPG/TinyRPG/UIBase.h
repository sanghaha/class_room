#pragma once

class UIBase
{
public:
	UIBase();
	virtual ~UIBase();
	
	virtual void Init() {}
	virtual void Render(ID2D1HwndRenderTarget* renderTarget);
	void Open() { _isOpen = true; }
	void Close() { _isOpen = false; }

protected:
	bool _isOpen = false;
};

