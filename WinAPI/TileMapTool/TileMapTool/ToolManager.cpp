#include "pch.h"
#include "ToolManager.h"
#include "TileMapMain.h"
#include "TileMapSub.h"


void ToolManager::Init(HWND hwndMain, HWND hwndSub)
{
	_hwndMain = hwndMain;
	_hwndSub = hwndSub;

	wchar_t buffer[MAX_PATH];
	DWORD length = ::GetCurrentDirectory(MAX_PATH, buffer);
	fs::path currentPath = fs::path(buffer) / L"../Resources/Tilemap_Elevation.bmp";

	_main = new TileMapMain();
	_main->Init(hwndMain, currentPath.c_str());

	_sub = new TileMapSub();
	_sub->Init(hwndSub, currentPath.c_str());
}

void ToolManager::Update()
{
	// 현재 활성화된 윈도우만 update()
	HWND hwnd = ::GetForegroundWindow();

	if (hwnd == _hwndMain && _main)
		_main->Update();

	if (hwnd == _hwndSub && _sub)
		_sub->Update();
}

void ToolManager::Render()
{
	if (_main) _main->Render();
	if (_sub) _sub->Render();
}

int ToolManager::GetSelectedTileIndex()
{
	if (_sub)
		return _sub->GetSelectedIndex();
	return -1;
}
