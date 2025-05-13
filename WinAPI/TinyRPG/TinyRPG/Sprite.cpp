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

	// �ҽ� ��Ʈ�ʿ��� ������ ���� ��ǥ ���
	float srcX = _indexX * (float)_bitmap->GetFrameSize().Width;
	float srcY = _indexY * (float)_bitmap->GetFrameSize().Height;

	// ���� ��Ʈ�ʿ��� �׸� ���� (�ҽ� ����)
	D2D1_RECT_F srcRect = D2D1::RectF(
		srcX + 1.0f,
		srcY + 1.0f,
		srcX + _bitmap->GetFrameSize().Width - 1.0f,
		srcY + _bitmap->GetFrameSize().Height - 1.0f);

	Vector renderPos = _applyCamera ? Game::ConvertScreenPos(pos) : pos;

	// ȭ�鿡 �������� ��ġ�� ũ�� (��� ����)
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

	// ���� ��ȯ ���� ����
	D2D1::Matrix3x2F originalTransform;
	renderTarget->GetTransform(&originalTransform);

	D2D1::Matrix3x2F finalTransform = originalTransform;
	D2D1::Matrix3x2F scaleTransform = D2D1::Matrix3x2F::Scale(
		D2D1::SizeF(1.0f, 1.0f),
		D2D1::Point2F(renderPos.x, renderPos.y) // ���� ������
	);
	finalTransform = scaleTransform * finalTransform;

	// �¿� ���� ��ȯ ��� ����
	if (_flip) // dirX�� ������ ��� �¿� ����
	{
		D2D1::Matrix3x2F flipTransform = D2D1::Matrix3x2F::Scale(
			D2D1::SizeF(-1.0f, 1.0f), // X�� ����, Y�� �״��
			D2D1::Point2F(renderPos.x, renderPos.y) // ���� ������
		);
		finalTransform = flipTransform * finalTransform;
	}
	// ȸ�� ��� ����
	if (_rotate != 0)
	{
		D2D1::Matrix3x2F rotate = D2D1::Matrix3x2F::Rotation(
			_rotate,
			D2D1::Point2F(renderPos.x, renderPos.y)
		);
		finalTransform = rotate * finalTransform;
	}

	// ���� ��ȯ
	renderTarget->SetTransform(finalTransform);

	// ��Ʈ�� ������
	renderTarget->DrawBitmap(_bitmap->GetBitmap(), destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, &srcRect);

	// ���� ��ȯ ���� ����
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
