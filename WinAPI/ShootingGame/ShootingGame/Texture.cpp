#include "pch.h"
#include "Texture.h"

Texture::Texture(wstring key, int32 width, int32 height)
	: Super(key, width, height)
{
}

Texture::~Texture()
{
}

void Texture::Render(HDC hdc, Pos pos)
{
	if (_bitmapInfo == nullptr)
		return;

	if (_bitmapInfo->transparent == -1)
	{
		::BitBlt(hdc,	// 백버퍼에
			(int32)pos.x,
			(int32)pos.y,
			_size.w,
			_size.h,
			_bitmapInfo->hdc,	// 텍스쳐 그리기
			0,
			0,
			SRCCOPY);
	}
	else
	{
		::TransparentBlt(hdc,
			(int32)pos.x,
			(int32)pos.y,
			_size.w,
			_size.h,
			_bitmapInfo->hdc,
			0,
			0,
			_size.w,
			_size.h,
			_bitmapInfo->transparent);
	}
}