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

	_hdcBack = ::CreateCompatibleDC(_hdc); // hdc�� ȣȯ�Ǵ� DC�� ����
	_bmpBack = ::CreateCompatibleBitmap(_hdc, _rect.right, _rect.bottom); // hdc�� ȣȯ�Ǵ� ��Ʈ�� ����
	HBITMAP prev = (HBITMAP)::SelectObject(_hdcBack, _bmpBack); // DC�� BMP�� ����
	::DeleteObject(prev);

	Gdiplus::GdiplusStartup(&_gdiplusToken, &_gdiplusStartupInput, NULL);
	
	SoundManager::GetInstance()->Init(hwnd);

	// ���ҽ� �Ŵ��� �ʱ�ȭ
	wchar_t buffer[MAX_PATH];
	DWORD length = ::GetCurrentDirectory(MAX_PATH, buffer);
	fs::path currentPath = fs::path(buffer) / L"../Resources/";
	ResourceManager::GetInstance()->Init(hwnd, currentPath);
	
	//������ �Ŵ��� �ʱ�ȭ
	DataManager::GetInstance()->Init();
	// Ÿ�̸� �ʱ�ȭ
	TimeManager::GetInstance()->Init();
	// �Է� �Ŵ��� �ʱ�ȭ
	InputManager::GetInstance()->Init(hwnd);

	// ���Ӿ� ����
	ChangeLobbyScene();
}

void Game::Destroy()
{
	if (_currScene)
	{
		_currScene->Destory();
		delete _currScene;
		_currScene = nullptr;
	}

	if (_nextScene)
	{
		delete _nextScene;
		_nextScene = nullptr;
	}

	if (_hdcBack)
		::DeleteDC(_hdcBack);

	if (_bmpBack)
		::DeleteObject(_bmpBack);

	if (_hdc)
		::ReleaseDC(_hwnd, _hdc);

	ResourceManager::DestroyInstance();
	TimeManager::DestroyInstance();
	InputManager::DestroyInstance();
	DataManager::DestroyInstance();
	SoundManager::DestroyInstance();

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
	::BitBlt(_hdc, 0, 0, _rect.right, _rect.bottom, _hdcBack, 0, 0, SRCCOPY); // ��Ʈ ���� : ���� ����
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
