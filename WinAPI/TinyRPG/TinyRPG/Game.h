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
	void OnLeftClickEvent();

	static Scene* GetScene();
	static class GameScene* GetGameScene();

	// camera ��ǥ ������ ���� ��ǥ
	static Vector ConvertScreenPos(Vector worldPos);
	static Vector ConvertWorldPos(Vector screenPos);
	static bool CanMove(Cell cell);


	ID2D1HwndRenderTarget* GetRenderTarget() { return _dxRenderTarget; }
	IWICImagingFactory* GetWICFactory() { return _wicFactory; }

protected:
	void Destroy() override;

private:
	void changeGameScene();

private:

	HWND	_hwnd;
	RECT	_rect;

	ID2D1Factory* _dxFactory = nullptr;
	ID2D1HwndRenderTarget* _dxRenderTarget = nullptr;
	IWICImagingFactory* _wicFactory = nullptr;

	Scene* _currScene = nullptr;
};

