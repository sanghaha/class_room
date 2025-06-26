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

	// camera ÁÂÇ¥ ±âÁØÀÇ ¿ùµå ÁÂÇ¥
	static Pos ConvertScreenPos(Pos worldPos);
	static Pos ConvertWorldPos(Pos screenPos);

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
};

