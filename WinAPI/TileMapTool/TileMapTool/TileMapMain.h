#pragma once
#include <windows.h>

class TileMapMain
{
public:
	void Init(HWND hwnd, std::wstring path);
	void Update();
	void Render();

private:
	void DrawMainGrid(HDC hdc);
	void DrawTileOnGrid(HDC hdc, int tileIndex, int gridX, int gridY);

	// ���� �Լ�
	void SaveTileMap();
	void LoadTileMap();

private:
	HWND	_hwnd;
	RECT	_rect;
	HDC		_hdc = {};
	HDC		_hdcBack = {};
	HBITMAP _bmpBack = {};

	HDC		_hdcBitmap = {};
	HBITMAP _bitmap = 0;
	int32 _transparent = 0;
	int32 _sizeX = 0;
	int32 _sizeY = 0;

	struct TileLayer
	{
		std::vector<int> mainGrid;
		int GetValidCount();
	};
	TileLayer _layer;


	bool _isDragging = false;
};

