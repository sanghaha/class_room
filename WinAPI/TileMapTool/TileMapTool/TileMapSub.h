#pragma once

class TileMapSub
{
public:
	void Init(HWND hwnd, std::wstring path);
	void Update();
	void Render();

	int GetSelectedIndex() const { return _selectedIndex; }

private:
	// 그리기 함수
	void DrawTileMap(HDC hdc);

private:
	RECT	_rect;
	HDC		_hdc = {};
	HDC		_hdcBack = {};
	HBITMAP _bmpBack = {};

	HDC		_hdcBitmap = {};
	HBITMAP _bitmap = 0;
	int32 _transparent = 0;
	int32 _sizeX = 0;
	int32 _sizeY = 0;

	int _selectedIndex = 0;
};

