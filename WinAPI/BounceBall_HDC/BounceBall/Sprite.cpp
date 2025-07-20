#include "pch.h"
#include "Sprite.h"
#include "Game.h"
#include "ResourceManager.h"
#include "DXBitmap.h"

Sprite::Sprite(wstring key, int32 width, int32 height, bool alignCenter) : Super(key, width, height), _alignCenter(alignCenter)
{
}

Sprite::~Sprite()
{
	
}

void Sprite::Render(HDC renderTarget, Vector pos)
{
	Size frameSize = _bitmap->GetFrameSize();

	// 소스 비트맵에서 복사할 시작 좌표 계산
	float srcX = _indexX * (float)frameSize.Width;
	float srcY = _indexY * (float)frameSize.Height;
	if (_bitmap->type == TextureType::PNG)
	{
		::Graphics g(renderTarget);

		g.SetInterpolationMode(Gdiplus::InterpolationModeNearestNeighbor);
		g.DrawImage(_bitmap->_img,
			pos.x, pos.y,
			srcX,
			srcY,
			(float)_bitmap->_frameSizeX,
			(float)_bitmap->_frameSizeY,
			Gdiplus::UnitPixel);
	}
	else
	{
		if (_bitmap->_transparent == -1)
		{
			::BitBlt(renderTarget,	// 백버퍼에
				(int32)pos.x - _size.Width * 0.5f,
				(int32)pos.y - _size.Height * 0.5f,
				_size.Width,
				_size.Height,
				_bitmap->bitmapHdc,	// 텍스쳐 그리기
				srcX,
				srcY,
				SRCCOPY);
		}
		else
		{
			::TransparentBlt(renderTarget,
				(int32)pos.x - _size.Width * 0.5f,
				(int32)pos.y - _size.Height * 0.5f,
				_size.Width,
				_size.Height,
				_bitmap->bitmapHdc,
				srcX,
				srcY,
				_bitmap->_frameSizeX,
				_bitmap->_frameSizeY,
				_bitmap->_transparent);
		}
	}
}

Size Sprite::GetFrameSize()
{
	if (_bitmap)
	{
		return _bitmap->GetFrameSize();
	}
	return Size();
}

void Sprite::GetFrameCount(int32& outX, int32& outY)
{
	outX = 0;
	outY = 0;

	if (_bitmap)
	{
		_bitmap->GetFrameCount(outX, outY);
	}
}
