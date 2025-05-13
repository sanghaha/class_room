#include "pch.h"
#include "Sprite.h"
#include "Game.h"
#include "ResourceManager.h"
#include "DXBitmap.h"

Sprite::Sprite(string key, int32 width, int32 height, bool alignCenter) : Super(key, width, height)
{
	_alignCenter = alignCenter;
}

Sprite::~Sprite()
{

}

void Sprite::Render(ID2D1RenderTarget* renderTarget, Vector pos)
{
	Size frameSize = _bitmap->GetFrameSize();

	// 소스 비트맵에서 복사할 시작 좌표 계산
	float srcX = _indexX * (float)_bitmap->GetFrameSize().Width;
	float srcY = _indexY * (float)_bitmap->GetFrameSize().Height;

	// 원본 비트맵에서 그릴 영역 (소스 영역)
	D2D1_RECT_F srcRect = D2D1::RectF(
		srcX + 1.0f,
		srcY + 1.0f,
		srcX + _bitmap->GetFrameSize().Width - 1.0f,
		srcY + _bitmap->GetFrameSize().Height - 1.0f);

	Vector renderPos = _applyCamera ? Game::ConvertScreenPos(pos) : pos;

	// 화면에 렌더링할 위치와 크기 (대상 영역)
	D2D1_RECT_F destRect;
	if (_alignCenter)
	{
		destRect = D2D1::RectF(
			roundf(renderPos.x - _size.Width * 0.5f),
			roundf(renderPos.y - _size.Height * 0.5f),
			roundf(renderPos.x + _size.Width * 0.5f),
			roundf(renderPos.y + _size.Height * 0.5f));
	}
	else
	{
		destRect = D2D1::RectF(
			roundf(renderPos.x),
			roundf(renderPos.y),
			roundf(renderPos.x + _size.Width),
			roundf(renderPos.y + _size.Height));
	}

	// 기존 변환 상태 저장
	D2D1::Matrix3x2F originalTransform;
	renderTarget->GetTransform(&originalTransform);

	D2D1::Matrix3x2F finalTransform = originalTransform;
	D2D1::Matrix3x2F scaleTransform = D2D1::Matrix3x2F::Scale(
		D2D1::SizeF(1.0f, 1.0f),
		D2D1::Point2F(renderPos.x, renderPos.y) // 반전 기준점
	);
	finalTransform = scaleTransform * finalTransform;

	// 좌우 반전 변환 행렬 설정
	if (_flip) // dirX가 음수일 경우 좌우 반전
	{
		D2D1::Matrix3x2F flipTransform = D2D1::Matrix3x2F::Scale(
			D2D1::SizeF(-1.0f, 1.0f), // X축 반전, Y축 그대로
			D2D1::Point2F(renderPos.x, renderPos.y) // 반전 기준점
		);
		finalTransform = flipTransform * finalTransform;
	}
	// 회전 행렬 설정
	if (_rotate != 0)
	{
		D2D1::Matrix3x2F rotate = D2D1::Matrix3x2F::Rotation(
			_rotate,
			D2D1::Point2F(renderPos.x, renderPos.y)
		);
		finalTransform = rotate * finalTransform;
	}

	// 최종 변환
	renderTarget->SetTransform(finalTransform);

	// 비트맵 렌더링
	renderTarget->DrawBitmap(_bitmap->GetBitmap(), destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, &srcRect);

	// 원래 변환 상태 복원
	renderTarget->SetTransform(originalTransform);

}

Size Sprite::GetFrameSize()
{
	return Size();
}

void Sprite::GetFrameCount(int32& outX, int32& outY)
{
	outX = 0;
	outY = 0;

	if (_bitmap)
	{
		_bitmap->GetFrameCount(outX, outY);
	}
}

//------------------------------------------------------------------------
