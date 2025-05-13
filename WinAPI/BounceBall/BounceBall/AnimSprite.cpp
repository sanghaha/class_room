#include "pch.h"
#include "AnimSprite.h"
#include "ResourceManager.h"
//
//AnimSprite::AnimSprite(wstring key, int32 width, int32 height, bool alignCenter) : Super(key, width, height, alignCenter)
//{
//}
//
//AnimSprite::~AnimSprite()
//{
//}
//
//void AnimSprite::Update(float deltaTime)
//{
//	if (_anim.IsEnd)
//		return;
//
//	_anim.SumTime += deltaTime;
//
//	int32 frameCountX = _anim.CountX;
//	int32 frameCountY = _anim.CountY;
//	int32 totalCount = frameCountX * frameCountY;
//	float delta = _anim.Duration / totalCount;
//
//	// ���� �ð��� ������ ���� ������ �̵�
//	if (_anim.SumTime >= delta && _anim.IsEnd == false)
//	{
//		_anim.IndexX = ((_anim.IndexX + 1 - _anim.StartX) % frameCountX) + _anim.StartX;
//
//		// x ������ �ѹ��� ���Ƽ� ������ �����ϸ� �������̵ȴ�.
//		if (_anim.StartX == _anim.IndexX)
//		{
//			_anim.IndexY = ((_anim.IndexY + 1 - _anim.StartY) % frameCountY) + _anim.StartY;
//		}
//
//		if (_anim.Loop == false)
//		{
//			if (_anim.StartX == _anim.IndexX && _anim.StartY == _anim.IndexY)
//			{
//				// ������ �ƴ� �ִϸ��̼��� ������ �����ӿ� �����ְ� �Ѵ�.
//				_anim.IndexX = _anim.StartX + _anim.CountX - 1;
//				_anim.IndexY = _anim.StartY + _anim.CountY - 1;
//				_anim.IsEnd = true;
//			}
//		}
//
//		_anim.SumTime -= delta;
//	}
//}
//
//void AnimSprite::Render(ID2D1RenderTarget* renderTarget, Vector pos)
//{
//	_indexX = _anim.IndexX;
//	_indexY = _anim.IndexY;
//
//	Super::Render(renderTarget, pos);
//}
