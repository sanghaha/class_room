#pragma once

class DXBitmap
{
public:
	DXBitmap();
	virtual ~DXBitmap();

	void Load(wstring path);
	ID2D1Bitmap* GetBitmap() { return _bitmap; }
	Size GetBitmapSize() { return Size(_bitmapSizeX, _bitmapSizeY); }

protected:
	uint32 _bitmapSizeX = 0;
	uint32 _bitmapSizeY = 0;
	ID2D1Bitmap* _bitmap = nullptr;
};