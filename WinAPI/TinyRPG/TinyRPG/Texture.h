#pragma once

#include "BaseBitmap.h"

class Sliced3Texture : public BaseBitmap
{
public:
	Sliced3Texture();
	virtual ~Sliced3Texture();

	void Render(ID2D1HwndRenderTarget* renderTarget, Vector pos, int32 sizeX, int32 sizeY, float clipRatio = 1.0f);
	void Load(wstring path, int32 left, int32 right);

private:
	int32 _left = 0;
	int32 _right = 0;
};

class PNGTexture : public BaseBitmap
{
public:
	PNGTexture();
	virtual ~PNGTexture();

	void Render(ID2D1HwndRenderTarget* renderTarget, Vector pos);
	void Load(wstring path, int32 width, int32 height);

private:
	int32 _width = 0;
	int32 _height = 0;
};
