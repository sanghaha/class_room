#pragma once

enum class TextureType
{
	BMP,
	PNG
};


class DXBitmap
{
public:
	DXBitmap();
	virtual ~DXBitmap();

	void Load(wstring path, int32 maxCountX, int32 maxCountY, int32 transparent);
	HDC GetBitmap() { return bitmapHdc; }
	Size GetBitmapSize() { return Size(_bitmapSizeX, _bitmapSizeY); }
	Size GetFrameSize() { return Size(_frameSizeX, _frameSizeY); }
	void GetFrameCount(int32& outX, int32& outY) { outX = _maxCountX; outY = _maxCountY; }

	TextureType type = TextureType::BMP;
	uint32	_bitmapSizeX = 0;
	uint32	_bitmapSizeY = 0;
	int32	_maxCountX = 0;
	int32	_maxCountY = 0;
	int32	_frameSizeX = 0;
	int32	_frameSizeY = 0;
	int32	_transparent = false;
	HDC			bitmapHdc = 0;
	HBITMAP		bitmap = 0;


	Gdiplus::Image* _img;
};