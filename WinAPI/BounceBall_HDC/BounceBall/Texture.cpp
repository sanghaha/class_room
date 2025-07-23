#include "pch.h"
#include "Texture.h"
#include "Game.h"

Texture::Texture()
{
}

Texture::~Texture()
{
}

void Texture::Load(wstring path, int32 maxCountX, int32 maxCountY, int32 transparent)
{
	HDC hdc = ::GetDC(Game::GetInstance()->GetHwnd());

	if (path.find(L".png") != std::wstring::npos)
	{
		type = TextureType::PNG;
		_img = Gdiplus::Image::FromFile(path.c_str());
		if (_img == nullptr)
		{
			return;
		}
		_bitmapSizeX = _img->GetWidth();
		_bitmapSizeY = _img->GetHeight();
	}
	else
	{
		type = TextureType::BMP;
		bitmapHdc = ::CreateCompatibleDC(hdc);
		bitmap = (HBITMAP)::LoadImageW(
			nullptr,
			path.c_str(),
			IMAGE_BITMAP,
			0,
			0,
			LR_LOADFROMFILE | LR_CREATEDIBSECTION
		);

		_transparent = transparent;

		if (bitmap == 0)
		{
			::MessageBox(Game::GetInstance()->GetHwnd(), path.c_str(), L"Invalid Texture Load", MB_OK);
			return;
		}

		HBITMAP prev = (HBITMAP)::SelectObject(bitmapHdc, bitmap);
		::DeleteObject(prev);

		BITMAP bit = {};
		::GetObject(bitmap, sizeof(BITMAP), &bit);

		_bitmapSizeX = bit.bmWidth;
		_bitmapSizeY = bit.bmHeight;
	}


	_maxCountX = maxCountX;
	_maxCountY = maxCountY;
	_frameSizeX = _bitmapSizeX / _maxCountX;
	_frameSizeY = _bitmapSizeY / _maxCountY;

	_sizeX = _frameSizeX;
	_sizeY = _frameSizeY;
}

void Texture::Render(HDC hdc, Vector pos, Vector srcPos)
{
	if (type == TextureType::PNG)
	{
		::Graphics g(hdc);
		g.SetInterpolationMode(Gdiplus::InterpolationModeNearestNeighbor);
		g.DrawImage(_img, pos.x, pos.y);
	}
	else
	{
		if (bitmapHdc == nullptr)
			return;

		Vector renderPos = _centerAlign ? Vector(pos.x - _sizeX * 0.5f, pos.y - _sizeY * 0.5f) : pos;

		if (_transparent == -1)
		{
			::BitBlt(hdc,	// 백버퍼에
				(int32)renderPos.x,
				(int32)renderPos.y,
				_sizeX,
				_sizeY,
				bitmapHdc,	// 텍스쳐 그리기
				srcPos.x,
				srcPos.y,
				SRCCOPY);
		}
		else
		{
			::TransparentBlt(hdc,
				(int32)renderPos.x,
				(int32)renderPos.y,
				_sizeX,
				_sizeY,
				bitmapHdc,
				srcPos.x,
				srcPos.y,
				_frameSizeX,
				_frameSizeY,
				_transparent);
		}
	}
}
