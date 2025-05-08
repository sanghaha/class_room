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
	Sprite(wstring key);
	virtual ~Sprite();

	virtual void Render(ID2D1RenderTarget* renderTarget, Vector pos);

	Size GetSize() { return Size(_info.width, _info.height); }

	void SetInfo(const SpriteRenderInfo& info);
	void SetIndex(int32 x, int32 y) { _info.indexX = x; _info.indexY = y; }
	void SetFlip(int8 flip) { _info.dirX = flip; }
protected:
	DXBitmap* _bitmap = nullptr;
	SpriteRenderInfo _info;
};


class NumberSprite : public Sprite
{
	using Super = Sprite;
public:
	NumberSprite(wstring key);
	virtual ~NumberSprite();

	void Render(ID2D1RenderTarget* renderTarget, Vector pos) override;

	void SetNumber(int8 number);
private:
	int8 _number = 0;
	std::vector<Vector> _numberPos;
};