#pragma once

#include "ResourceBase.h"

class Sprite : public ResourceBase
{
	using Super = ResourceBase;

public:
	Sprite(wstring key, int32 width, int32 height, int32 indexX, int32 indexY);
	virtual ~Sprite();

	void Render(HDC hdc, Pos pos) override;
	void SetIndex(int32 x, int32 y) { _indexX = x; _indexY = y; }
	Size GetSize() { return _renderSize; }

private:
	int32 _indexX = 0;
	int32 _indexY = 0;
	Size _renderSize = {};
};



