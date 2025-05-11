#pragma once
#include "BaseResource.h"


class Sprite : public BaseResource
{
	using Super = BaseResource;

public:
	Sprite(wstring key, int32 width, int32 height, bool alignCenter);
	virtual ~Sprite();

	void Render(ID2D1RenderTarget* renderTarget, Vector pos) override;
	void SetIndex(int32 x, int32 y) { _indexX = x; _indexY = y; }
	void GetIndex(int32& outX, int32& outY) { outX = _indexX; outY = _indexY; }
	Size GetFrameSize();
	void GetFrameCount(int32& outX, int32& outY);

protected:
	int32 _indexX = 0;
	int32 _indexY = 0;
	bool _alignCenter = true;
};