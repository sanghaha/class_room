#pragma once
#include "Singleton.h"

class Scene;
class Game : public Singleton<Game>
{
	// Singleton<T>�� Game�� protected/private ��� ���� �����ϵ���
	friend class Singleton<Game>;

public:
	Game();
	~Game();

	void Init(HWND hwnd);
	void Update();
	void Render();

	static Scene* GetScene();

	HWND GetHwnd() { return _hwnd; }
	ID2D1HwndRenderTarget* GetRenderTarget() { return _dxRenderTarget; }
	IWICImagingFactory* GetWICFactory() { return _wicFactory; }
	
	void ChangeLobbyScene();
	void ChangeGameScene();
	void ChangeEditorScene();

protected:
	void Destroy() override;

private:

	HWND	_hwnd;
	RECT	_rect;

	ID2D1Factory* _dxFactory = nullptr;
	ID2D1HwndRenderTarget* _dxRenderTarget = nullptr;
	IWICImagingFactory* _wicFactory = nullptr;

	Scene* _currScene = nullptr;
	Scene* _nextScene = nullptr;
};

