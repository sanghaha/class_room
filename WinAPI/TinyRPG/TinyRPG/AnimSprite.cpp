#include "pch.h"
#include "AnimSprite.h"
#include "Sprite.h"


void AnimInfo::Update(float deltaTime)
{
	if (IsEnd)
		return;

	SumTime += deltaTime;

	int32 frameCountX = CountX;
	int32 frameCountY = CountY;
	int32 totalCount = frameCountX * frameCountY;
	float delta = Duration / totalCount;

	// 일정 시간이 지나면 다음 프레임 이동
	if (SumTime >= delta && IsEnd == false)
	{
		IndexX = ((IndexX + 1 - StartX) % frameCountX) + StartX;

		// x 개수가 한바퀴 돌아서 끝까지 도착하면 시작점이된다.
		if (StartX == IndexX)
		{
			IndexY = ((IndexY + 1 - StartY) % frameCountY) + StartY;
		}

		if (Loop == false)
		{
			if (StartX == IndexX && StartY == IndexY)
			{
				// 루프가 아닌 애니메이션은 마지막 프레임에 멈춰있게 한다.
				IndexX = StartX + CountX - 1;
				IndexY = StartY + CountY - 1;
				IsEnd = true;
			}
		}

		SumTime -= delta;
	}
}

void AnimInfo::Reset()
{
	IndexX = StartX;
	IndexY = StartY;
	IsEnd = false;
}


/// <summary>
/// 
/// </summary>
AnimSprite::AnimSprite()
{
}

AnimSprite::~AnimSprite()
{
}

void AnimSprite::Update(float deltaTime)
{
	if (_sprite == nullptr)
		return;
	if (_info == nullptr)
		return;

	_info->Update(deltaTime);
}

void AnimSprite::Render(HDC hdc, Vector pos)
{
	if (_sprite == nullptr)
		return;
	if (_info == nullptr)
		return;

	_sprite->Render(hdc, pos, _info->IndexX, _info->IndexY, _info->FlipX);
	_lastRenderFlipX = _info->FlipX;
}

void AnimSprite::SetSprite(Sprite* sprite)
{
	_sprite = sprite;
}

void AnimSprite::SetAnimInfo(AnimInfo* info)
{
	_info = info;
}

Size AnimSprite::GetRenderSize()
{
	if (_sprite)
		return _sprite->GetSize();
	return Size{ 0,0 };
}

