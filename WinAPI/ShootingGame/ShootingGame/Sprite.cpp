#include "pch.h"
#include "Sprite.h"
#include "Game.h"

Sprite::Sprite(wstring key, int32 width, int32 height, int32 indexX, int32 indexY)
	: Super(key, width, height)
{
	_indexX = indexX;
	_indexY = indexY;

	if (_bitmapInfo)
	{
		int32 renderSizeX = _bitmapInfo->countX != 0 ? (_size.w / _bitmapInfo->countX) : _size.w;
		int32 renderSizeY = _bitmapInfo->countY != 0 ? (_size.h / _bitmapInfo->countY) : _size.h;
		_renderSize.w = renderSizeX;
		_renderSize.h = renderSizeY;
	}
}

Sprite::~Sprite()
{
}

void Sprite::Render(HDC hdc, Pos pos)
{
	if (nullptr == _bitmapInfo)
		return;

	// 소스 비트맵에서 복사할 시작 좌표 계산
	int32 srcX = _indexX * _renderSize.w;
	int32 srcY = _indexY * _renderSize.h;

	Pos renderPos = _applyCamera ? Game::ConvertScreenPos(pos) : pos;

	if (_bitmapInfo->transparent == -1)
	{
		::BitBlt(hdc,	// 백버퍼에
			(int32)renderPos.x,
			(int32)renderPos.y,
			_renderSize.w,
			_renderSize.h,
			_bitmapInfo->hdc,	// 텍스쳐 그리기
			srcX,
			srcY,
			SRCCOPY);
	}
	else
	{
		::TransparentBlt(hdc,
			(int32)renderPos.x,
			(int32)renderPos.y,
			_renderSize.w,
			_renderSize.h,
			_bitmapInfo->hdc,
			srcX,
			srcY,
			_renderSize.w,
			_renderSize.h,
			_bitmapInfo->transparent);
	}
}
