#include "pch.h"
#include "Texture.h"
#include "Game.h"

Texture::Texture()
{
	_img = nullptr;
}

Texture::~Texture()
{
	if (_flipBitmapHdc) ::DeleteDC(_flipBitmapHdc);
	if (_flipBitmap)    ::DeleteObject(_flipBitmap);
	if (bitmapHdc)
		::DeleteDC(bitmapHdc);
	if (bitmap)
		::DeleteObject(bitmap);
	if (_img)
		delete _img;
}

void Texture::Load(wstring path, int32 maxCountX, int32 maxCountY, int32 transparent, bool enableFlip)
{
	HDC hdc = ::GetDC(Game::GetInstance()->GetHwnd());

	if (path.find(L".png") != std::wstring::npos)
	{
		type = TextureType::PNG;
		_img = Gdiplus::Image::FromFile(path.c_str());
		if (_img == nullptr)
		{
			::ReleaseDC(Game::GetInstance()->GetHwnd(), hdc);
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
			::ReleaseDC(Game::GetInstance()->GetHwnd(), hdc);
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

	if (enableFlip && type == TextureType::BMP && bitmapHdc)
	{
		_flipBitmapHdc = ::CreateCompatibleDC(hdc);
		_flipBitmap    = ::CreateCompatibleBitmap(hdc, _bitmapSizeX, _bitmapSizeY);
		::SelectObject(_flipBitmapHdc, _flipBitmap);
		::StretchBlt(_flipBitmapHdc,
			_bitmapSizeX - 1, 0, -(int32)_bitmapSizeX, _bitmapSizeY,
			bitmapHdc, 0, 0, _bitmapSizeX, _bitmapSizeY,
			SRCCOPY);
	}

	::ReleaseDC(Game::GetInstance()->GetHwnd(), hdc);
}

void Texture::Render(HDC hdc, Vector pos, Vector srcPos, bool flipX)
{
	if (type == TextureType::PNG)
	{
		//::Graphics g(hdc);
		//g.SetInterpolationMode(Gdiplus::InterpolationModeNearestNeighbor);
		//g.DrawImage(_img, pos.x, pos.y);
	}
	else
	{
		if (bitmapHdc == nullptr)
			return;

		Vector renderPos = _centerAlign ? Vector(pos.x - _sizeX * 0.5f, pos.y - _sizeY * 0.5f) : pos;

		HDC   srcDC = bitmapHdc;
		int32 srcX  = (int32)srcPos.x;
		int32 srcY  = (int32)srcPos.y;

		if (flipX && _flipBitmapHdc)
		{
			srcDC = _flipBitmapHdc;
			srcX  = _bitmapSizeX - (int32)srcPos.x - _frameSizeX;
		}

		if (_transparent == -1)
		{
			::BitBlt(hdc,
				(int32)renderPos.x, (int32)renderPos.y, _sizeX, _sizeY,
				srcDC, srcX, srcY, SRCCOPY);
		}
		else
		{
			::TransparentBlt(hdc,
				(int32)renderPos.x, (int32)renderPos.y, _sizeX, _sizeY,
				srcDC, srcX, srcY, _frameSizeX, _frameSizeY,
				_transparent);
		}
	}
}
