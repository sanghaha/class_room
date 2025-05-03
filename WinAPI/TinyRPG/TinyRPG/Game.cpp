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
#include "DataManager.h"
#include "InventorySystem.h"

Game::Game()
{
	_currScene = new EmptyScene();
}

Game::~Game()
{

}

void Game::Init(HWND hwnd)
{
	srand((uint32)time(0));
	_hwnd = hwnd;

	//_hdc = ::GetDC(hwnd);

	::GetClientRect(hwnd, &_rect);

	//_hdcBack = ::CreateCompatibleDC(_hdc); // hdc와 호환되는 DC를 생성
	//_bmpBack = ::CreateCompatibleBitmap(_hdc, _rect.right, _rect.bottom); // hdc와 호환되는 비트맵 생성
	//HBITMAP prev = (HBITMAP)::SelectObject(_hdcBack, _bmpBack); // DC와 BMP를 연결
	//::DeleteObject(prev);

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
	changeGameScene();

	// 인벤토리 초기화
	InventorySystem::GetInstance()->Init();
}

void Game::Destroy()
{
	ResourceManager::DestroyInstance();
	TimeManager::DestroyInstance();
	InputManager::DestroyInstance();
	DataManager::DestroyInstance();

	if (_currScene)
	{
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
	TimeManager::GetInstance()->Update();
	InputManager::GetInstance()->Update();
	ResourceManager::GetInstance()->Update(TimeManager::GetDeltaTime());

	GetScene()->Update(TimeManager::GetDeltaTime());

	UIManager::GetInstance()->Update();
}

void Game::Render()
{
	_dxRenderTarget->BeginDraw();
	_dxRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

	GetScene()->Render(_dxRenderTarget);

	uint32 fps = TimeManager::GetInstance()->GetFps();
	float deltaTime = TimeManager::GetDeltaTime();

	auto brush = ResourceManager::GetInstance()->GetBrush(BrushColor::White);
	auto font = ResourceManager::GetInstance()->GetFont(FontSize::FONT_12);

	{
		POINT mousePos = InputManager::GetInstance()->GetMousePos();
		wstring str = std::format(L"Mouse({0}, {1})", mousePos.x, mousePos.y);
		_dxRenderTarget->DrawTextW(
			str.c_str(),
			(uint32)str.size(),
			font,
			D2D1::RectF(300, 10, 600, 200),
			brush
		);
	}

	{
		wstring str = std::format(L"FPS({0}), DT({1})", fps, deltaTime);
		_dxRenderTarget->DrawTextW(
			str.c_str(),
			(uint32)str.size(),
			font,
			D2D1::RectF(5, 10, 600, 200),
			brush
		);
	}

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

GameScene* Game::GetGameScene()
{
	if (GetInstance())
	{
		return dynamic_cast<GameScene*>(GetInstance()->_currScene);
	}
	return nullptr;
}

Vector Game::ConvertScreenPos(Vector worldPos)
{
	if (GetScene())
	{
		Vector cameraPos = GetScene()->GetCameraPos();
		Vector pos;
		pos.x = worldPos.x - (cameraPos.x - GWinSizeX / 2);
		pos.y = worldPos.y - (cameraPos.y - GWinSizeY / 2);
		return pos;
	}
	return worldPos;
}

Vector Game::ConvertWorldPos(Vector screenPos)
{
	if (GetScene())
	{
		Vector cameraPos = GetScene()->GetCameraPos();
		Vector pos;
		pos.x = screenPos.x + (cameraPos.x - GWinSizeX / 2);
		pos.y = screenPos.y + (cameraPos.y - GWinSizeY / 2);
		return pos;
	}
	return screenPos;
}

bool Game::CanMove(Cell cell)
{
	if (GetGameScene())
	{
		return GetGameScene()->CanMove(cell);
	}
	return false;
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
