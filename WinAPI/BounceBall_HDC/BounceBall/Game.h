#pragma once
#include "Singleton.h"

class Scene;
class Game : public Singleton<Game>
{
	// Singleton<T>이 Game의 protected/private 멤버 접근 가능하도록
	friend class Singleton<Game>;

public:
	Game();
	~Game();

	void Init(HWND hwnd);
	void Update();
	void Render();

	static Scene* GetScene();

	HWND GetHwnd() { return _hwnd; }
	
	void ChangeLobbyScene();
	void ChangeGameScene();
	void ChangeEditorScene();

	static bool STOP_WATCH;
	static bool UPDATE_FRAME;

protected:
	void Destroy() override;

private:

	HWND	_hwnd;
	RECT	_rect;
	HDC		_hdc = {};

	// Double Buffering
	HDC		_hdcBack = {};
	HBITMAP _bmpBack = {};

	// GDI+
	ULONG_PTR _gdiplusToken;
	GdiplusStartupInput  _gdiplusStartupInput;

	Scene* _currScene = nullptr;
	Scene* _nextScene = nullptr;


};

