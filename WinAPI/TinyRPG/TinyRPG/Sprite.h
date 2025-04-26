#pragma once

class Sprite
{
public:
	Sprite();
	virtual ~Sprite();

	virtual void Render(HDC hdc, Vector pos, int32 indexX, int32 indexY, int32 dirX) abstract;

	int32 GetMaxCountX() const { return _maxCountX; }
	int32 GetMaxCountY() const { return _maxCountY; }
	Size GetSize() { return _renderSize; }

protected:
	int32 _sizeX = 0;
	int32 _sizeY = 0;
	int32 _maxCountX = 0;
	int32 _maxCountY = 0;
	Size _renderSize = {};
};

class BitmapSprite : public Sprite
{
public:
	BitmapSprite();
	virtual ~BitmapSprite();

	void Load(HWND hwnd, wstring path, int32 transparent, int32 maxCountX, int32 maxCountY);
	void Render(HDC hdc, Vector pos, int32 indexX, int32 indexY, int32 dirX) override;

private:
	HDC _hdc = 0;
	HBITMAP _bitmap = 0;
	int32 _transparent = 0;
};


class PNGSprite : public Sprite
{
public:
	PNGSprite();
	virtual ~PNGSprite();

	void Load(HWND hwnd, wstring path, int32 maxCountX, int32 maxCountY);
	void Render(HDC hdc, Vector pos, int32 indexX, int32 indexY, int32 dirX) override;

private:
	Image* _image = nullptr;
};
