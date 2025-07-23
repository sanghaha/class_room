#pragma once

enum class TextureType
{
	BMP,
	PNG
};


class Texture
{
public:
	Texture();
	virtual ~Texture();

	void Load(wstring path, int32 maxCountX, int32 maxCountY, int32 transparent);
	void Render(HDC hdc, Vector pos, Vector srcPos);

	HDC GetBitmap() { return bitmapHdc; }
	Size GetBitmapSize() { return Size(_bitmapSizeX, _bitmapSizeY); }
	Size GetFrameSize() { return Size(_frameSizeX, _frameSizeY); }
	void GetFrameCount(int32& outX, int32& outY) { outX = _maxCountX; outY = _maxCountY; }

	void SetSize(int32 x, int32 y) { _sizeX = x; _sizeY = y; }
	void SetCenterAlign(bool center) { _centerAlign = center; }

private:

	TextureType type = TextureType::BMP;
	uint32	_bitmapSizeX = 0;
	uint32	_bitmapSizeY = 0;
	int32	_maxCountX = 0;
	int32	_maxCountY = 0;
	int32	_frameSizeX = 0;
	int32	_frameSizeY = 0;
	int32	_transparent = -1;

	int32	_sizeX = 0;
	int32	_sizeY = 0;
	bool	_centerAlign = true;

	HDC			bitmapHdc = 0;
	HBITMAP		bitmap = 0;
	Gdiplus::Image* _img;
};