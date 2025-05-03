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
	// �ҽ� ��Ʈ�ʿ��� ������ ���� ��ǥ ���
	float srcX = info.indexX * (float)_renderSize.Width;
	float srcY = info.indexY * (float)_renderSize.Height;

	// ���� ��Ʈ�ʿ��� �׸� ���� (�ҽ� ����)
	D2D1_RECT_F srcRect = D2D1::RectF(
		srcX + 1.0f,
		srcY + 1.0f,
		srcX + _renderSize.Width - 1.0f,
		srcY + _renderSize.Height - 1.0f);

	Vector renderPos = info.applyCamera ? Game::ConvertRenderPos(pos) : pos;

	// ȭ�鿡 �������� ��ġ�� ũ�� (��� ����)
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

	// ���� ��ȯ ���� ����
	D2D1_MATRIX_3X2_F originalTransform;
	renderTarget->GetTransform(&originalTransform);

	D2D1_MATRIX_3X2_F finalTransform = originalTransform;

	// �¿� ���� ��ȯ ��� ����
	if (info.dirX < 0) // dirX�� ������ ��� �¿� ����
	{
		D2D1_MATRIX_3X2_F flipTransform = D2D1::Matrix3x2F::Scale(
			D2D1::SizeF(-1.0f, 1.0f), // X�� ����, Y�� �״��
			D2D1::Point2F(renderPos.x, renderPos.y) // ���� ������
		);
		finalTransform = flipTransform * finalTransform;
	}
	// ȸ�� ��� ����
	if (info.rotate != 0)
	{
		D2D1::Matrix3x2F rotate = D2D1::Matrix3x2F::Rotation(
			info.rotate,
			D2D1::Point2F(renderPos.x, renderPos.y)
		);
		finalTransform = rotate * finalTransform;
	}

	// ���� ��ȯ
	renderTarget->SetTransform(finalTransform);

	// ��Ʈ�� ������
	renderTarget->DrawBitmap(_bitmap, destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, &srcRect);

	// ���� ��ȯ ���� ����
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
