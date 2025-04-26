#include "pch.h"
#include "BitmapTexture.h"
#include "Game.h"

BitmapTexture::BitmapTexture()
{
}

BitmapTexture::~BitmapTexture()
{
}

void BitmapTexture::Render(HDC hdc, Vector pos, int32 srcX, int32 srcY)
{
	Vector renderPos = Game::ConvertRenderPos(pos);
	if (_transparent == -1)
	{
		::BitBlt(hdc,	// 백버퍼에
			(int32)renderPos.x,
			(int32)renderPos.y,
			_sizeX,
			_sizeY,
			_hdc,	// 텍스쳐 그리기
			srcX,
			srcY,
			SRCCOPY);
	}
	else
	{
		::TransparentBlt(hdc,
			(int32)renderPos.x,
			(int32)renderPos.y,
			_sizeX,
			_sizeY,
			_hdc,
			srcX,
			srcY,
			_sizeX,
			_sizeY,
			_transparent);
	}
}

void BitmapTexture::Load(HWND hwnd, wstring path, int32 transparent)
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
		::MessageBox(hwnd, path.c_str(), L"Invalid BitmapTexture Load", MB_OK);
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
