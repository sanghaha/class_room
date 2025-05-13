#pragma once
#include "BaseResource.h"

class Sprite : public BaseResource
{
	using Super = BaseResource;

public:
	Sprite(string key, int32 width, int32 height, bool alignCenter);
	virtual ~Sprite();

	void Render(ID2D1RenderTarget* renderTarget, Vector pos) override;
	void SetIndex(int32 x, int32 y) { _indexX = x; _indexY = y; }
	void GetIndex(int32& outX, int32& outY) { outX = _indexX; outY = _indexY; }
	void SetFlip(bool flip) { _flip = flip; }
	bool GetFlip() { return _flip; }
	void SetApplyCamera(bool apply) { _applyCamera = apply; }
	void SetRotate(float rotate) { _rotate = rotate; }

	Size GetFrameSize();
	void GetFrameCount(int32& outX, int32& outY);

protected:
	int32 _indexX = 0;
	int32 _indexY = 0;
	float _rotate = 0;
	bool _flip = false;
	bool _alignCenter = true;
	bool _applyCamera = true;
};