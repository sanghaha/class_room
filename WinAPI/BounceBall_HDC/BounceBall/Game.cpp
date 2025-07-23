#include "pch.h"
#include "Game.h"
#include "Scene.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "UIManager.h"
#include "EmptyScene.h"
#include "LobbyScene.h"
#include "GameScene.h"
#include "EditorScene.h"
#include "DataManager.h"
#include "SoundManager.h"

bool Game::STOP_WATCH = false;
bool Game::UPDATE_FRAME = false;

Game::Game()
{
	_nextScene = new EmptyScene();
}

Game::~Game()
{

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

	Gdiplus::GdiplusStartup(&_gdiplusToken, &_gdiplusStartupInput, NULL);
	
	SoundManager::GetInstance()->Init(hwnd);

	// 리소스 매니저 초기화
	wchar_t buffer[MAX_PATH];
	DWORD length = ::GetCurrentDirectory(MAX_PATH, buffer);
	fs::path currentPath = fs::path(buffer) / L"../Resources/";
	ResourceManager::GetInstance()->Init(hwnd, currentPath);
	
	//데이터 매니저 초기화
	DataManager::GetInstance()->Init();
	// 타이머 초기화
	TimeManager::GetInstance()->Init();
	// 입력 매니저 초기화
	InputManager::GetInstance()->Init(hwnd);

	// 게임씬 생성
	ChangeLobbyScene();
}

void Game::Destroy()
{
	ResourceManager::DestroyInstance();
	TimeManager::DestroyInstance();
	InputManager::DestroyInstance();
	DataManager::DestroyInstance();

	if (_currScene)
	{
		_currScene->Destory();
		delete _currScene;
		_currScene = nullptr;
	}

	GdiplusShutdown(_gdiplusToken);
}

void Game::Update()
{
	if (_nextScene)
	{
		if (_currScene)
		{
			_currScene->Destory();
			delete _currScene;
			_currScene = nullptr;
		}

		_currScene = _nextScene;
		_currScene->Init();

		_nextScene = nullptr;
	}

	TimeManager::GetInstance()->Update();
	InputManager::GetInstance()->Update();
	ResourceManager::GetInstance()->Update(TimeManager::GetDeltaTime());

	if (STOP_WATCH)
	{
		if (UPDATE_FRAME)
		{
			GetScene()->Update(TimeManager::GetDeltaTime());
			UPDATE_FRAME = false;
		}
	}
	else
	{
		GetScene()->Update(TimeManager::GetDeltaTime());
	}

	if (InputManager::GetInstance()->GetButtonDown(KeyType::F3))
	{
		STOP_WATCH = !STOP_WATCH;
		UPDATE_FRAME = false;
	}
	if (STOP_WATCH && InputManager::GetInstance()->GetButtonDown(KeyType::LeftMouse))
	{
		UPDATE_FRAME = true;
	}
}

void Game::Render()
{
	GetScene()->Render(_hdcBack);

	POINT pos = InputManager::GetInstance()->GetMousePos();
	wstring str = std::format(L"Mouse:{0}, {1}", pos.x, pos.y);
	TextOut(_hdcBack, 5, 10, str.c_str(), static_cast<int32>(str.size()));

	// Double Buffering
	::BitBlt(_hdc, 0, 0, _rect.right, _rect.bottom, _hdcBack, 0, 0, SRCCOPY); // 비트 블릿 : 고속 복사
	::PatBlt(_hdcBack, 0, 0, _rect.right, _rect.bottom, WHITENESS);
}

Scene* Game::GetScene()
{
	if (GetInstance())
	{
		return GetInstance()->_currScene;
	}
	return nullptr;
}

void Game::ChangeLobbyScene()
{
	if (_nextScene)
	{
		delete _nextScene;
		_nextScene = nullptr;
	}

	_nextScene = new LobbyScene();
}

void Game::ChangeGameScene()
{
	if (_nextScene)
	{
		delete _nextScene;
		_nextScene = nullptr;
	}

	_nextScene = new GameScene();
}

void Game::ChangeEditorScene()
{
	if (_nextScene)
	{
		delete _nextScene;
		_nextScene = nullptr;
	}

	_nextScene = new EditorScene();
}
