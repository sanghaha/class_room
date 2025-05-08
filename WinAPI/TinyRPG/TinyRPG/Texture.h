#pragma once

#include "DXBitmap.h"

class Sliced3Texture
{
public:
	Sliced3Texture(wstring key, int32 left, int32 right);
	virtual ~Sliced3Texture();

	void Render(ID2D1RenderTarget* renderTarget, Vector pos, int32 sizeX, int32 sizeY, float clipRatio = 1.0f);

private:
	DXBitmap* _bitmap = nullptr;
	int32 _left = 0;
	int32 _right = 0;
};

class PNGTexture
{
public:
	PNGTexture(wstring key, int32 width = 0, int32 height = 0);
	virtual ~PNGTexture();

	void Render(ID2D1RenderTarget* renderTarget, Vector pos);

private:
	DXBitmap* _bitmap = nullptr;
	Size		_size = {};
};
