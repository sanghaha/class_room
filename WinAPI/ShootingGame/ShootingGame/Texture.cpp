#include "pch.h"
#include "Texture.h"

Texture::Texture()
{
}

Texture::~Texture()
{
}

void Texture::Render(HDC hdc, Pos pos)
{
	if (_transparent == -1)
	{
		::BitBlt(hdc,	// 백버퍼에
			(int32)pos.x,
			(int32)pos.y,
			_sizeX,
			_sizeY,
			_hdc,	// 텍스쳐 그리기
			0,
			0,
			SRCCOPY);
	}
	else
	{
		::TransparentBlt(hdc,
			(int32)pos.x,
			(int32)pos.y,
			_sizeX,
			_sizeY,
			_hdc,
			0,
			0,
			_sizeX,
			_sizeY,
			_transparent);
	}
}

void Texture::Load(HWND hwnd, wstring path, int32 transparent)
{
	HDC hdc = ::GetDC(hwnd);

	_hdc = ::CreateCompatibleDC(hdc);
	_bitmap = (HBITMAP)::LoadImageW(
		nullptr,
		path.c_str(),
		IMAGE_BITMAP,
		0,
		0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION
	);

	if (_bitmap == 0)
	{
		::MessageBox(hwnd, path.c_str(), L"Invalid Texture Load", MB_OK);
		return;
	}

	HBITMAP prev = (HBITMAP)::SelectObject(_hdc, _bitmap);
	::DeleteObject(prev);

	BITMAP bit = {};
	::GetObject(_bitmap, sizeof(BITMAP), &bit);

	_sizeX = bit.bmWidth;
	_sizeY = bit.bmHeight;
	_transparent = transparent;
}
