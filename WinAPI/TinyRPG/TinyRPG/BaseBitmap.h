#pragma once

class BaseBitmap
{
public:
	BaseBitmap();
	virtual ~BaseBitmap();

protected:
	void loadBitmap(wstring path);

protected:
	uint32 _sizeX = 0;
	uint32 _sizeY = 0;
	ID2D1Bitmap* _bitmap = nullptr;
};

