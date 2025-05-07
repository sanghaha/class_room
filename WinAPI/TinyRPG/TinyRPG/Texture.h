#pragma once

#include "DXBitmap.h"

class Sliced3Texture
{
public:
	Sliced3Texture();
	virtual ~Sliced3Texture();

	void Render(ID2D1HwndRenderTarget* renderTarget, Vector pos, int32 sizeX, int32 sizeY, float clipRatio = 1.0f);
	void Load(wstring path, int32 left, int32 right);

private:
	DXBitmap* _bitmap = nullptr;
	int32 _left = 0;
	int32 _right = 0;
};

class PNGTexture
{
public:
	PNGTexture();
	virtual ~PNGTexture();

	void Render(ID2D1HwndRenderTarget* renderTarget, Vector pos);
	void Load(wstring path, int32 width, int32 height);

private:
	DXBitmap* _bitmap = nullptr;
	Size		_size = {};
};
