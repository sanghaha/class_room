#include "pch.h"
#include "DXBitmap.h"
#include "Game.h"

DXBitmap::DXBitmap()
{
}

DXBitmap::~DXBitmap()
{
}

void DXBitmap::Load(wstring path, int32 maxCountX, int32 maxCountY, int32 transparent)
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
}
