#include "pch.h"
#include "Game.h"
#include "Scene.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "CollisionManager.h"
#include "UIManager.h"
#include "DataManager.h"
#include "EmptyScene.h"
#include "GameScene.h"

Game::Game()
{
	_currScene = new EmptyScene();
}

Game::~Game()
{
	ResourceManager::DestroyInstance();
	TimeManager::DestroyInstance();
	InputManager::DestroyInstance();
	CollisionManager::DestroyInstance();
	UIManager::DestroyInstance();
	DataManager::DestroyInstance();

	if (_currScene)
	{
		_currScene->Destory();
		delete _currScene;
		_currScene = nullptr;
	}
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

	// 리소스 매니저 초기화
	wchar_t buffer[MAX_PATH];
	DWORD length = ::GetCurrentDirectory(MAX_PATH, buffer);
	fs::path currentPath = fs::path(buffer) / L"../Resources/";
	ResourceManager::GetInstance()->Init(hwnd, currentPath);

	// 데이터 매니저 초기화
	DataManager::GetInstance()->Init();

	// 게임씬 생성
	changeGameScene();
	
	// 타이머 초기화
	TimeManager::GetInstance()->Init();
	// 입력 매니저 초기화
	InputManager::GetInstance()->Init(hwnd);
	// 충돌 매니저 초기화
	CollisionManager::GetInstance()->Init();
	// UI 매니저 초기화
	UIManager::GetInstance()->Init();
}

void Game::Update()
{
	TimeManager::GetInstance()->Update();
	InputManager::GetInstance()->Update();
	ResourceManager::GetInstance()->Update(TimeManager::GetDeltaTime());

	GetScene()->Update(TimeManager::GetDeltaTime());

	// 충돌 체크에 대한 업데이트 순서도 중요하다. 모든 로직이 끝나고 나중에 최종 업데이트를 한다.
	CollisionManager::GetInstance()->Update();
}

void Game::Render()
{
	GetScene()->Render(_hdcBack);

	CollisionManager::GetInstance()->Render(_hdcBack);

	uint32 fps = TimeManager::GetInstance()->GetFps();
	float deltaTime = TimeManager::GetDeltaTime();

	HFONT hOldFont = (HFONT)SelectObject(_hdcBack, ResourceManager::GetInstance()->GetFont());
	
	//wchar_t fontName[LF_FACESIZE];
	//GetTextFace(_hdcBack, LF_FACESIZE, fontName);
	//MessageBox(_hwnd, fontName, L"현재 적용된 폰트 이름", MB_OK);

	{
		POINT mousePos = InputManager::GetInstance()->GetMousePos();
		wstring str = std::format(L"Mouse({0}, {1})", mousePos.x, mousePos.y);
		::TextOut(_hdcBack, 300, 10, str.c_str(), static_cast<int32>(str.size()));
	}

	{
		wstring str = std::format(L"FPS({0}), DT({1})", fps, deltaTime);
		::TextOut(_hdcBack, 5, 10, str.c_str(), static_cast<int32>(str.size()));
	}

	//DeleteObject(hOldFont);

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

GameScene* Game::GetGameScene()
{
	if (GetInstance())
	{
		return dynamic_cast<GameScene*>(GetInstance()->_currScene);
	}
	return nullptr;
}

Pos Game::ConvertScreenPos(Pos worldPos)
{
	if (GetScene())
	{
		Pos cameraPos = GetScene()->GetCameraPos();
		if (cameraPos.x != 0 && cameraPos.y != 0)
		{
			Pos pos;
			pos.x = worldPos.x - (cameraPos.x - GWinSizeX / 2);
			pos.y = worldPos.y - (cameraPos.y - GWinSizeY / 2);
			return pos;
		}
	}
	return worldPos;
}

Pos Game::ConvertWorldPos(Pos screenPos)
{
	if (GetScene())
	{
		Pos cameraPos = GetScene()->GetCameraPos();
		Pos pos;
		pos.x = screenPos.x + (cameraPos.x - GWinSizeX / 2);
		pos.y = screenPos.y + (cameraPos.y - GWinSizeY / 2);
		return pos;
	}
	return screenPos;
}

void Game::changeGameScene()
{
	if (_currScene)
	{
		delete _currScene;
		_currScene = nullptr;
	}

	_currScene = new GameScene();
	_currScene->Init();
}
