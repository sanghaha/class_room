#include "pch.h"
#include "Texture.h"
#include "Game.h"

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

	Pos renderPos = _applyCamera ? Game::ConvertScreenPos(pos) : pos;

	if (_bitmapInfo->transparent == -1)
	{
		::BitBlt(hdc,	// 백버퍼에
			(int32)renderPos.x,
			(int32)renderPos.y,
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
			(int32)renderPos.x,
			(int32)renderPos.y,
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