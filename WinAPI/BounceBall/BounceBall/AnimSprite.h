#pragma once
#include "Sprite.h"

class AnimInfo
{
public:
	int32 IndexX = 0;
	int32 IndexY = 0;
	int32 StartX = 0;
	int32 StartY = 0;
	int32 CountX = 0;
	int32 CountY = 0;
	bool Loop = false;
	float Duration = 0.2f;
	bool IsEnd = false;
	float SumTime = 0;
};

//class AnimSprite : public Sprite
//{
//	using Super = Sprite;
//public:
//	AnimSprite(wstring key, int32 width, int32 height, bool alignCenter);
//	virtual ~AnimSprite();
//
//	void Update(float deltaTime) override;
//	void Render(ID2D1RenderTarget* renderTarget, Vector pos) override;
//
//	bool IsEnd() { return _anim.IsEnd; }
//
//private:
//	AnimInfo	_anim;
//};
//
