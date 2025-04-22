#pragma once

#include "Singleton.h"

class TileMapMain;
class TileMapSub;

class ToolManager : public Singleton<ToolManager>
{
public:
	void Init(HWND hwndMain, HWND hwndSub);
	void Update();
	void Render();
	int GetSelectedTileIndex();

private:
	HWND _hwndMain = 0;
	HWND _hwndSub = 0;

	TileMapMain* _main = nullptr;
	TileMapSub* _sub = nullptr;
};

