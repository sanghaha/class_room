#pragma once

class Texture
{
public:
	Texture() {}
	virtual ~Texture() {}
};

class BitmapTexture : public Texture
{
public:
	BitmapTexture();
	virtual ~BitmapTexture();
	void Render(ID2D1HwndRenderTarget* renderTarget, Vector pos, int32 srcX, int32 srcY);

	void Load(HWND hwnd, wstring path, int32 transparent);
	Size GetSize() { return Size{ _sizeX, _sizeY }; }

private:
	int32 _sizeX = 0;
	int32 _sizeY = 0;
	int32 _transparent = 0;
};


class Sliced3Texture : public Texture
{
public:
	Sliced3Texture();
	virtual ~Sliced3Texture();
	void Render(ID2D1HwndRenderTarget* renderTarget, Vector pos, int32 sizeX, int32 sizeY, float clipRatio = 1.0f);

	void Load(HWND hwnd, wstring path, int32 left, int32 right);
	Size GetSize() { return Size{ _sizeX, _sizeY }; }

private:
	int32 _sizeX = 0;
	int32 _sizeY = 0;
	int32 _left = 0;
	int32 _right = 0;
};
