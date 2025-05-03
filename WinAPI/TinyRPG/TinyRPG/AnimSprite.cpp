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

	// ���� �ð��� ������ ���� ������ �̵�
	if (SumTime >= delta && IsEnd == false)
	{
		IndexX = ((IndexX + 1 - StartX) % frameCountX) + StartX;

		// x ������ �ѹ��� ���Ƽ� ������ �����ϸ� �������̵ȴ�.
		if (StartX == IndexX)
		{
			IndexY = ((IndexY + 1 - StartY) % frameCountY) + StartY;
		}

		if (Loop == false)
		{
			if (StartX == IndexX && StartY == IndexY)
			{
				// ������ �ƴ� �ִϸ��̼��� ������ �����ӿ� �����ְ� �Ѵ�.
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

