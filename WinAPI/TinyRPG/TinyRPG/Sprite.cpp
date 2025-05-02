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

void Sprite::Load(wstring path, int32 maxCountX, int32 maxCountY, bool alignCenter)
{
	loadBitmap(path);

	_maxCountX = maxCountX;
	_maxCountY = maxCountY;
	_alignCenter = alignCenter;

	int32 renderSizeX = _maxCountX != 0 ? (_sizeX / _maxCountX) : _sizeX;
	int32 renderSizeY = _maxCountY != 0 ? (_sizeY / _maxCountY) : _sizeY;
	_renderSize.Width = renderSizeX;
	_renderSize.Height = renderSizeY;
}

void Sprite::Render(ID2D1HwndRenderTarget* renderTarget, Vector pos, int32 indexX, int32 indexY, int32 dirX)
{
	// �ҽ� ��Ʈ�ʿ��� ������ ���� ��ǥ ���
	float srcX = indexX * (float)_renderSize.Width;
	float srcY = indexY * (float)_renderSize.Height;

	// ���� ��Ʈ�ʿ��� �׸� ���� (�ҽ� ����)
	D2D1_RECT_F srcRect = D2D1::RectF(
		srcX + 1.0f,
		srcY + 1.0f,
		srcX + _renderSize.Width - 1.0f,
		srcY + _renderSize.Height - 1.0f);

	Vector renderPos = Game::ConvertRenderPos(pos);

	// ȭ�鿡 �������� ��ġ�� ũ�� (��� ����)
	D2D1_RECT_F destRect;
	if (_alignCenter)
	{
		destRect = D2D1::RectF(
			roundf(renderPos.x - _renderSize.Width * 0.5f),
			roundf(renderPos.y - _renderSize.Height * 0.5f),
			roundf(renderPos.x + _renderSize.Width * 0.5f),
			roundf(renderPos.y + _renderSize.Height * 0.5f));
	}
	else
	{
		destRect = D2D1::RectF(
			roundf(renderPos.x),
			roundf(renderPos.y),
			roundf(renderPos.x + _renderSize.Width),
			roundf(renderPos.y + _renderSize.Height));
	}

	// ���� ��ȯ ���� ����
	D2D1_MATRIX_3X2_F originalTransform;
	renderTarget->GetTransform(&originalTransform);

	// �¿� ���� ��ȯ ��� ����
	if (dirX < 0) // dirX�� ������ ��� �¿� ����
	{
		D2D1_MATRIX_3X2_F flipTransform = D2D1::Matrix3x2F::Scale(
			D2D1::SizeF(-1.0f, 1.0f), // X�� ����, Y�� �״��
			D2D1::Point2F(renderPos.x, renderPos.y) // ���� ������
		);
		renderTarget->SetTransform(flipTransform * originalTransform);
	}

	// ��Ʈ�� ������
	renderTarget->DrawBitmap(_bitmap, destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, &srcRect);

	// ���� ��ȯ ���� ����
	renderTarget->SetTransform(originalTransform);

}
