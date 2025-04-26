#pragma once


class Sprite;

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

	AnimInfo() {}
	AnimInfo(int32 startX, int32 startY, int32 countX, int32 countY, bool loop, float dur) 
		: IndexX(startX), IndexY(startY), StartX(startX), StartY(startY), CountX(countX), CountY(countY), Loop(loop), Duration(dur), SumTime(0)
	{}

	void Update(float deltaTime);
	void Reset();
};

class AnimSprite
{
public:
	AnimSprite();
	virtual ~AnimSprite();

	void Update(float deltaTime);
	void Render(HDC hdc, Vector pos, int32 dirX);

	void SetSprite(Sprite* sprite);
	void SetAnimInfo(AnimInfo* info);
	bool IsEnd() { if (_info) return _info->IsEnd; return false; }

	Size GetRenderSize();

private:
	Sprite* _sprite = nullptr;
	AnimInfo* _info = nullptr;
};

