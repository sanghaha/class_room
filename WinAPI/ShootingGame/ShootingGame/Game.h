#pragma once
#include "Singleton.h"

class Scene;
class Game : public Singleton<Game>
{
public:
	Game();
	~Game();

	void Init(HWND hwnd);
	void Update();
	void Render();

	static Scene* GetScene();
	static class GameScene* GetGameScene();
	wstring GetName() override { return L"Game"; };
	HWND	GetHwnd() { return _hwnd; }

	// camera 좌표 기준의 월드 좌표
	static Pos ConvertScreenPos(Pos worldPos);
	static Pos ConvertWorldPos(Pos screenPos);

	// Camera
	Pos GetCameraPos() { return _cameraPos; }
	void SetCameraPos(Pos pos) { _cameraPos = pos; }

private:
	void changeGameScene();

private:

	HWND	_hwnd;
	HDC		_hdc;

	// Double Buffering
	RECT	_rect;
	HDC		_hdcBack = {};
	HBITMAP _bmpBack = {};

	Scene* _currScene = nullptr;

	Pos _cameraPos = { 0, 0 };
};

