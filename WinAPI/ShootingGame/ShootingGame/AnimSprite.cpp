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

	_sumTime += deltaTime;

	// 일정 시간이 지나면 다음 프레임 이동
	if (_sumTime >= _time && _isEnd == false)
	{
		if (_indexX + 1 >= _sprite->GetCountX())
		{
			_indexX = 0;

			if (_indexY + 1 >= _sprite->GetCountY())
			{
				_indexY = 0;

				if (_sprite->IsLoop() == false)
					_isEnd = true;
			}
			else
				++_indexY;
		}
		else
		{
			++_indexX;
		}

		_sumTime -= _time;
	}
}

void AnimSprite::Render(HDC hdc, Pos pos)
{
	if (_sprite == nullptr)
		return;

	_sprite->Render(hdc, pos, _indexX, _indexY);
}

void AnimSprite::SetSprite(Sprite* sprite, float frameTime)
{
	_sprite = sprite;
	_time = frameTime;
}

void AnimSprite::Reset()
{
	_indexX = 0;
	_indexY = 0;
	_sumTime = 0;
	_isEnd = false;
}
