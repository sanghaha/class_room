#include "pch.h"
#include "Sprite.h"
#include "Game.h"
#include "ResourceManager.h"

Sprite::Sprite()
{
}

Sprite::~Sprite()
{
	
}

void Sprite::Load(wstring path, int32 maxCountX, int32 maxCountY)
{
	_bitmap = ResourceManager::GetInstance()->LoadDXBitmap(path);

	_maxCountX = maxCountX;
	_maxCountY = maxCountY;

	_frameSizeX = _maxCountX != 0 ? (_bitmap->GetBitmapSize().Width / _maxCountX) : _bitmap->GetBitmapSize().Width;
	_frameSizeY = _maxCountY != 0 ? (_bitmap->GetBitmapSize().Height / _maxCountY) : _bitmap->GetBitmapSize().Height;
	_info.width = _frameSizeX;
	_info.height = _frameSizeY;
}

void Sprite::Render(ID2D1HwndRenderTarget* renderTarget, Vector pos)
{
	// 소스 비트맵에서 복사할 시작 좌표 계산
	float srcX = _info.indexX * (float)_frameSizeX;
	float srcY = _info.indexY * (float)_frameSizeY;

	// 원본 비트맵에서 그릴 영역 (소스 영역)
	D2D1_RECT_F srcRect = D2D1::RectF(
		srcX + 1.0f,
		srcY + 1.0f,
		srcX + _frameSizeX - 1.0f,
		srcY + _frameSizeY - 1.0f);

	Vector renderPos = _info.applyCamera ? Game::ConvertScreenPos(pos) : pos;

	// 화면에 렌더링할 위치와 크기 (대상 영역)
	D2D1_RECT_F destRect;
	if (_info.alignCenter)
	{
		destRect = D2D1::RectF(
			roundf(renderPos.x - _info.width * 0.5f),
			roundf(renderPos.y - _info.height * 0.5f),
			roundf(renderPos.x + _info.width * 0.5f),
			roundf(renderPos.y + _info.height * 0.5f));
	}
	else
	{
		destRect = D2D1::RectF(
			roundf(renderPos.x),
			roundf(renderPos.y),
			roundf(renderPos.x + _info.width),
			roundf(renderPos.y + _info.height));
	}

	// 기존 변환 상태 저장
	D2D1::Matrix3x2F originalTransform;
	renderTarget->GetTransform(&originalTransform);

	D2D1::Matrix3x2F finalTransform = originalTransform;
	D2D1::Matrix3x2F scaleTransform = D2D1::Matrix3x2F::Scale(
		D2D1::SizeF(_info.scale, _info.scale),
		D2D1::Point2F(renderPos.x, renderPos.y) // 반전 기준점
	);
	finalTransform = scaleTransform * finalTransform;

	// 좌우 반전 변환 행렬 설정
	if (_info.dirX < 0) // dirX가 음수일 경우 좌우 반전
	{
		D2D1::Matrix3x2F flipTransform = D2D1::Matrix3x2F::Scale(
			D2D1::SizeF(-1.0f, 1.0f), // X축 반전, Y축 그대로
			D2D1::Point2F(renderPos.x, renderPos.y) // 반전 기준점
		);
		finalTransform = flipTransform * finalTransform;
	}
	// 회전 행렬 설정
	if (_info.rotate != 0)
	{
		D2D1::Matrix3x2F rotate = D2D1::Matrix3x2F::Rotation(
			_info.rotate,
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

void Sprite::SetInfo(const SpriteRenderInfo& info)
{
	_info = info;
	if (info.width == 0)
		_info.width = _frameSizeX;
	if (info.height == 0)
		_info.height = _frameSizeY;
}
