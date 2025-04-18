#include "pch.h"
#include "Sprite.h"

Sprite::Sprite()
{
}

Sprite::~Sprite()
{
}

void Sprite::Render(HDC hdc, Pos pos, int32 indexX, int32 indexY)
{
	// 소스 비트맵에서 복사할 시작 좌표 계산
	int32 srcX = indexX * _renderSize.w;
	int32 srcY = indexY * _renderSize.h;

	if (_transparent == -1)
	{
		::BitBlt(hdc,	// 백버퍼에
			(int32)pos.x,
			(int32)pos.y,
			_renderSize.w,
			_renderSize.h,
			_hdc,	// 텍스쳐 그리기
			srcX,
			srcY,
			SRCCOPY);
	}
	else
	{
		::TransparentBlt(hdc,
			(int32)pos.x,
			(int32)pos.y,
			_renderSize.w,
			_renderSize.h,
			_hdc,
			srcX,
			srcY,
			_renderSize.w,
			_renderSize.h,
			_transparent);
	}
}

void Sprite::Load(HWND hwnd, wstring path, int32 transparent, int32 countX, int32 countY, bool loop)
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
	_countX = countX;
	_countY = countY;
	_loop = loop;

	int32 renderSizeX = _countX != 0 ? (_sizeX / _countX) : _sizeX;
	int32 renderSizeY = _countY != 0 ? (_sizeY / _countY) : _sizeY;
	_renderSize.w = renderSizeX;
	_renderSize.h = renderSizeY;
}
