#include "pch.h"
#include "AnimSprite.h"
#include "Sprite.h"

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

	_sumTime += deltaTime;

	int32 frameCountX = _info->CountX;
	int32 frameCountY = _info->CountY;
	int32 totalCount = frameCountX * frameCountY;
	float delta = _info->Duration / totalCount;

	// ���� �ð��� ������ ���� ������ �̵�
	if (_sumTime >= delta && _info->IsEnd == false)
	{
		_info->IndexX = ((_info->IndexX+1) % frameCountX) + _info->StartX;

		// x ������ �ѹ��� ���Ƽ� ������ �����ϸ� �������̵ȴ�.
		if (_info->StartX == _info->CountX)
		{
			_info->IndexY = ((_info->IndexY + 1) % frameCountY) + _info->StartY;
		}

		if (_info->Loop == false)
		{
			if (_info->StartX == _info->CountX && _info->StartY == _info->CountY)
			{
				_info->IsEnd = true;
			}
		}

		_sumTime -= delta;
	}
}

void AnimSprite::Render(HDC hdc, Pos pos, int32 dirX)
{
	if (_sprite == nullptr)
		return;
	if (_info == nullptr)
		return;

	_sprite->Render(hdc, pos, _info->IndexX, _info->IndexY, dirX);
}

void AnimSprite::SetSprite(Sprite* sprite)
{
	_sprite = sprite;
}

void AnimSprite::SetAnimInfo(AnimInfo* info)
{
	Reset();
	_info = info;
}

void AnimSprite::Reset()
{
	if (_info == nullptr)
		return;

	_info->IndexX = _info->StartX;
	_info->IndexY = _info->StartY;
	_info->IsEnd = false;
}

Size AnimSprite::GetRenderSize()
{
	if (_sprite)
		return _sprite->GetSize();
	return Size{ 0,0 };
}
