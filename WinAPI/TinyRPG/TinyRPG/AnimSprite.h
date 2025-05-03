#pragma once
#include "Sprite.h"

class Sprite;

enum AnimType
{
	A_IDLE,
	A_MOVE,
	A_ATTACK,
	A_DEAD,
	A_MAX
};

class AnimInfo
{
public:
	int32 IndexX = 0;
	int32 IndexY = 0;
	int32 StartX = 0;
	int32 StartY = 0;
	int32 CountX = 0;
	int32 CountY = 0;
	int8 FlipX = 1;
	bool Loop = false;
	float Duration = 0.2f;
	bool IsEnd = false;
	float SumTime = 0;

	AnimInfo() {}
	AnimInfo(int32 startX, int32 startY, int32 countX, int32 countY, bool loop, float dur, int8 flipX = 1) 
		: IndexX(startX), IndexY(startY), StartX(startX), StartY(startY), CountX(countX), CountY(countY), Loop(loop), Duration(dur), SumTime(0), FlipX(flipX)
	{}

	void Update(float deltaTime);
	void Reset();
};

class AnimSpriteRenderer
{
public:
	AnimSpriteRenderer();
	virtual ~AnimSpriteRenderer();

	void Update(float deltaTime);
	void Render(ID2D1HwndRenderTarget* renderTarget, Vector pos);

	void SetSprite(Sprite* sprite);
	void SetAnimInfo(AnimInfo* info);
	bool IsEnd() { if (_info) return _info->IsEnd; return false; }

	Size GetRenderSize();
	AnimInfo* GetAimInfo() const { return _info; }
	int8 GetLastRenderFlipX() const { return _lastRenderFlipX; }

private:
	Sprite* _sprite = nullptr;
	AnimInfo* _info = nullptr;
	int8 _lastRenderFlipX = 1;
	SpriteRenderInfo _renderInfo;
};

