#include "pch.h"
#include "AnimSprite.h"

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
AnimSpriteRenderer::AnimSpriteRenderer()
{
}

AnimSpriteRenderer::~AnimSpriteRenderer()
{
}

void AnimSpriteRenderer::Update(float deltaTime)
{
	if (_sprite == nullptr)
		return;
	if (_info == nullptr)
		return;

	_info->Update(deltaTime);
}

void AnimSpriteRenderer::Render(ID2D1HwndRenderTarget* renderTarget, Vector pos)
{
	if (_sprite == nullptr)
		return;
	if (_info == nullptr)
		return;

	_renderInfo.indexX = _info->IndexX;
	_renderInfo.indexY = _info->IndexY;
	_renderInfo.dirX = _info->FlipX;

	_sprite->Render(renderTarget, pos, _renderInfo);
	_lastRenderFlipX = _info->FlipX;
}

void AnimSpriteRenderer::SetSprite(Sprite* sprite)
{
	_sprite = sprite;

	if (_renderInfo.width == 0)
		_renderInfo.width = _sprite->GetSize().Width;
	if (_renderInfo.height == 0)
		_renderInfo.height = _sprite->GetSize().Height;
}

void AnimSpriteRenderer::SetAnimInfo(AnimInfo* info)
{
	_info = info;
}

Size AnimSpriteRenderer::GetRenderSize()
{
	if (_sprite)
		return _sprite->GetSize();
	return Size{ 0,0 };
}

