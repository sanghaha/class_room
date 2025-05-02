#pragma once

class Sprite
{
public:
	Sprite();
	virtual ~Sprite();

	void Load(wstring path, int32 maxCountX, int32 maxCountY, bool alignCenter = true);
	void Render(ID2D1HwndRenderTarget* renderTarget, Vector pos, int32 indexX, int32 indexY, int32 dirX);

	int32 GetMaxCountX() const { return _maxCountX; }
	int32 GetMaxCountY() const { return _maxCountY; }
	Size GetSize() { return _renderSize; }

protected:
	uint32 _sizeX = 0;
	uint32 _sizeY = 0;
	int32 _maxCountX = 0;
	int32 _maxCountY = 0;
	Size _renderSize = {};
	int32 _transparent = 0;
	ID2D1Bitmap* _bitmap = nullptr;
	bool _alignCenter = true;
};