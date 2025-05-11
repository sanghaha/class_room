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

	::GetClientRect(hwnd, &_rect);

	// dx init
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &_dxFactory);
	
	D2D1_SIZE_U size = D2D1::SizeU(_rect.right - _rect.left, _rect.bottom - _rect.top);

	// Create a Direct2D render target.
	_dxFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(hwnd, size),
		&_dxRenderTarget);

	// 이미지 로드 초기화
	CoInitialize(NULL);
	CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&_wicFactory));


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

	SAFE_RELEASE(_dxFactory);
	SAFE_RELEASE(_dxRenderTarget);
	SAFE_RELEASE(_wicFactory);

	CoUninitialize();
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

	GetScene()->Update(TimeManager::GetDeltaTime());
}

void Game::Render()
{
	_dxRenderTarget->BeginDraw();
	_dxRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

	GetScene()->Render(_dxRenderTarget);

	_dxRenderTarget->EndDraw();
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
