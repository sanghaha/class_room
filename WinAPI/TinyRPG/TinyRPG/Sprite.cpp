#include "pch.h"
#include "Sprite.h"
#include "Game.h"

Sprite::Sprite()
{
}

Sprite::~Sprite()
{
	SAFE_RELEASE(_bitmap);
}

void Sprite::Load(wstring path, int32 maxCountX, int32 maxCountY)
{
	loadBitmap(path);

	_maxCountX = maxCountX;
	_maxCountY = maxCountY;

	int32 renderSizeX = _maxCountX != 0 ? (_sizeX / _maxCountX) : _sizeX;
	int32 renderSizeY = _maxCountY != 0 ? (_sizeY / _maxCountY) : _sizeY;
	_renderSize.Width = renderSizeX;
	_renderSize.Height = renderSizeY;
}

void Sprite::Render(ID2D1HwndRenderTarget* renderTarget, Vector pos, const SpriteRenderInfo& info)
{
	// 소스 비트맵에서 복사할 시작 좌표 계산
	float srcX = info.indexX * (float)_renderSize.Width;
	float srcY = info.indexY * (float)_renderSize.Height;

	// 원본 비트맵에서 그릴 영역 (소스 영역)
	D2D1_RECT_F srcRect = D2D1::RectF(
		srcX + 1.0f,
		srcY + 1.0f,
		srcX + _renderSize.Width - 1.0f,
		srcY + _renderSize.Height - 1.0f);

	Vector renderPos = info.applyCamera ? Game::ConvertRenderPos(pos) : pos;

	// 화면에 렌더링할 위치와 크기 (대상 영역)
	D2D1_RECT_F destRect;
	if (info.alignCenter)
	{
		destRect = D2D1::RectF(
			roundf(renderPos.x - info.width * 0.5f),
			roundf(renderPos.y - info.height * 0.5f),
			roundf(renderPos.x + info.width * 0.5f),
			roundf(renderPos.y + info.height * 0.5f));
	}
	else
	{
		destRect = D2D1::RectF(
			roundf(renderPos.x),
			roundf(renderPos.y),
			roundf(renderPos.x + info.width),
			roundf(renderPos.y + info.height));
	}

	// 기존 변환 상태 저장
	D2D1_MATRIX_3X2_F originalTransform;
	renderTarget->GetTransform(&originalTransform);

	D2D1_MATRIX_3X2_F finalTransform = originalTransform;

	// 좌우 반전 변환 행렬 설정
	if (info.dirX < 0) // dirX가 음수일 경우 좌우 반전
	{
		D2D1_MATRIX_3X2_F flipTransform = D2D1::Matrix3x2F::Scale(
			D2D1::SizeF(-1.0f, 1.0f), // X축 반전, Y축 그대로
			D2D1::Point2F(renderPos.x, renderPos.y) // 반전 기준점
		);
		finalTransform = flipTransform * finalTransform;
	}
	// 회전 행렬 설정
	if (info.rotate != 0)
	{
		D2D1::Matrix3x2F rotate = D2D1::Matrix3x2F::Rotation(
			info.rotate,
			D2D1::Point2F(renderPos.x, renderPos.y)
		);
		finalTransform = rotate * finalTransform;
	}

	// 최종 변환
	renderTarget->SetTransform(finalTransform);

	// 비트맵 렌더링
	renderTarget->DrawBitmap(_bitmap, destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, &srcRect);

	// 원래 변환 상태 복원
	renderTarget->SetTransform(originalTransform);

}


//--------------------------------------------------
SpriteRenderer::SpriteRenderer()
{
}

SpriteRenderer::~SpriteRenderer()
{
}

void SpriteRenderer::SetSprite(Sprite* sprite, const SpriteRenderInfo& info)
{
	_sprite = sprite;
	_renderInfo = info;

	if (_renderInfo.width == 0)
		_renderInfo.width = _sprite->GetSize().Width;
	if (_renderInfo.height == 0)
		_renderInfo.height = _sprite->GetSize().Height;
}

void SpriteRenderer::Render(ID2D1HwndRenderTarget* renderTarget, Vector pos)
{
	if (_sprite == nullptr)
		return;

	_sprite->Render(renderTarget, pos, _renderInfo);
}
