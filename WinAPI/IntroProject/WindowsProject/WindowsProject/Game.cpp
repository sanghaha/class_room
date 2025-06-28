#include "pch.h"
#include "Game.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "Player.h"
#include "EditScene.h"
#include "GameScene.h"


void Game::Destroy()
{
	TimeManager::DestroyInstance();
	InputManager::DestroyInstance();
	ResourceManager::DestroyInstance();
}

void Game::Init(HWND hwnd)
{
	srand((uint32)time(0));
	_hwnd = hwnd;

	_hdc = ::GetDC(hwnd);

	::GetClientRect(hwnd, &_rect);

	_hdcBack = ::CreateCompatibleDC(_hdc); // hdc와 호환되는 DC를 생성
	_bmpBack = ::CreateCompatibleBitmap(_hdc, _rect.right, _rect.bottom); // hdc와 호환되는 비트맵 생성
	HBITMAP prev = (HBITMAP)::SelectObject(_hdcBack, _bmpBack); // DC와 BMP를 연결
	::DeleteObject(prev);

	// 타이머 초기화
	TimeManager::GetInstance()->Init();
	// 입력 매니저 초기화
	InputManager::GetInstance()->Init(hwnd);

	ResourceManager::GetInstance()->Init();

	_currScene = new GameScene();
	_currScene->Init();
}

void Game::Update()
{
	TimeManager::GetInstance()->Update();
	InputManager::GetInstance()->Update();

	if (_currScene)
		_currScene->Update(TimeManager::GetInstance()->GetDeltaTime());
}

void Game::Render()
{
	if (_currScene)
		_currScene->Render(_hdcBack);



	uint32 fps = TimeManager::GetInstance()->GetFps();
	float deltaTime = TimeManager::GetDeltaTime();

	{
		POINT mousePos = InputManager::GetInstance()->GetMousePos();
		wstring str = std::format(L"Mouse({0}, {1})", mousePos.x, mousePos.y);
		::TextOut(_hdcBack, 300, 10, str.c_str(), static_cast<int32>(str.size()));
	}

	{
		wstring str = std::format(L"FPS({0}), DT({1})", fps, deltaTime);
		::TextOut(_hdcBack, 5, 10, str.c_str(), static_cast<int32>(str.size()));
	}

	// Double Buffering
	::BitBlt(_hdc, 0, 0, _rect.right, _rect.bottom, _hdcBack, 0, 0, SRCCOPY); // 비트 블릿 : 고속 복사
	::PatBlt(_hdcBack, 0, 0, _rect.right, _rect.bottom, WHITENESS);
}
