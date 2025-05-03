#pragma once
#include "BaseBitmap.h"

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
};

class Sprite : public BaseBitmap
{
public:
	Sprite();
	virtual ~Sprite();

	void Load(wstring path, int32 maxCountX, int32 maxCountY);
	void Render(ID2D1HwndRenderTarget* renderTarget, Vector pos, const SpriteRenderInfo& info);

	int32 GetMaxCountX() const { return _maxCountX; }
	int32 GetMaxCountY() const { return _maxCountY; }
	Size GetSize() { return _renderSize; }

protected:
	int32	_maxCountX = 0;
	int32	_maxCountY = 0;
	Size	_renderSize = {};
};

class SpriteRenderer
{
public:
	SpriteRenderer();
	virtual ~SpriteRenderer();

	void SetSprite(Sprite* sprite, const SpriteRenderInfo& info);
	Sprite* GetSprite() const { return _sprite; }
	void Render(ID2D1HwndRenderTarget* renderTarget, Vector pos);
	void SetIndex(int32 x, int32 y) { _renderInfo.indexX = x; _renderInfo.indexY = y; }
private:
	Sprite* _sprite = nullptr;
	SpriteRenderInfo _renderInfo;
};