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
	if (_anim == nullptr)
		return;

	_anim->Update(deltaTime);
}

void AnimSpriteRenderer::Render(ID2D1HwndRenderTarget* renderTarget, Vector pos)
{
	if (_sprite == nullptr)
		return;
	if (_anim == nullptr)
		return;

	_sprite->SetIndex(_anim->IndexX, _anim->IndexY);
	_sprite->SetFlip(_anim->FlipX);
	_sprite->Render(renderTarget, pos);
	_lastRenderFlipX = _anim->FlipX;
}

void AnimSpriteRenderer::SetSprite(Sprite* sprite)
{
	_sprite = sprite;
}

void AnimSpriteRenderer::SetAnimInfo(AnimInfo* info)
{
	_anim = info;
}

Size AnimSpriteRenderer::GetRenderSize()
{
	if (_sprite)
		return _sprite->GetSize();
	return Size{ 0,0 };
}

