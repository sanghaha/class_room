#pragma once

class DXBitmap
{
public:
	DXBitmap();
	virtual ~DXBitmap();

	void Load(wstring path, int32 maxCountX, int32 maxCountY, bool transparent);
	ID2D1Bitmap* GetBitmap() { return _bitmap; }
	Size GetBitmapSize() { return Size(_bitmapSizeX, _bitmapSizeY); }
	Size GetFrameSize() { return Size(_frameSizeX, _frameSizeY); }
	void GetFrameCount(int32& outX, int32& outY) { outX = _maxCountX; outY = _maxCountY; }

protected:
	uint32	_bitmapSizeX = 0;
	uint32	_bitmapSizeY = 0;
	int32	_maxCountX = 0;
	int32	_maxCountY = 0;
	int32	_frameSizeX = 0;
	int32	_frameSizeY = 0;
	bool	_transparent = false;
	ID2D1Bitmap* _bitmap = nullptr;
};