#pragma once
#include "DXBitmap.h"

struct SpriteRenderInfo
{
	int32 indexX = 0;
	int32 indexY = 0;
	int32 dirX = 1;
	int32 rotate = 0;
	int32 width = 0;
	int32 height = 0;
	bool alignCenter = true;
	bool applyCamera = true;
	float scale = 1.0f;
};

class Sprite
{
public:
	Sprite();
	virtual ~Sprite();

	void Load(wstring path, int32 maxCountX, int32 maxCountY);
	void Render(ID2D1HwndRenderTarget* renderTarget, Vector pos);

	int32 GetMaxCountX() const { return _maxCountX; }
	int32 GetMaxCountY() const { return _maxCountY; }
	Size GetSize() { return Size(_info.width, _info.height); }

	void SetInfo(const SpriteRenderInfo& info);
	void SetIndex(int32 x, int32 y) { _info.indexX = x; _info.indexY = y; }
	void SetFlip(int8 flip) { _info.dirX = flip; }
protected:
	DXBitmap* _bitmap = nullptr;
	SpriteRenderInfo _info;

	int32	_maxCountX = 0;
	int32	_maxCountY = 0;
	int32	_frameSizeX = 0;
	int32	_frameSizeY = 0;
};