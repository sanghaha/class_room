#pragma once

class BitmapTexture
{
public:
	BitmapTexture();
	virtual ~BitmapTexture();
	void Render(HDC hdc, Pos pos, int32 srcX, int32 srcY);

	void Load(HWND hwnd, wstring path, int32 transparent);
	Size GetSize() { return Size{ _sizeX, _sizeY }; }

private:
	int32 _sizeX = 0;
	int32 _sizeY = 0;
	HDC _hdc = 0;
	HBITMAP _bitmap = 0;
	int32 _transparent = 0;
};

