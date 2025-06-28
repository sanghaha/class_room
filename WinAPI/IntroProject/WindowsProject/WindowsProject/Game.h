#pragma once
#include "Singleton.h"

class Scene;
class Game : public Singleton<Game>
{
	friend Singleton<Game>;
protected:
	virtual void Destroy() override;

public:

	void Init(HWND hwnd);
	void Update();
	void Render();

	HWND GetHwnd() { return _hwnd; }

private:

	HWND	_hwnd;
	HDC		_hdc;

	// Double Buffering
	RECT	_rect;
	HDC		_hdcBack = {};
	HBITMAP _bmpBack = {};

	class Scene* _currScene = nullptr;

};