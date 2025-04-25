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

	// camera 좌표 기준의 월드 좌표
	static Pos ConvertRenderPos(Pos localPos);

protected:
	void Destroy() override;

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

