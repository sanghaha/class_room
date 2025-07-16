#include "pch.h"
#include "Sprite.h"
#include "Game.h"
#include "ResourceManager.h"
#include "DXBitmap.h"

Sprite::Sprite(wstring key, int32 width, int32 height, bool alignCenter) : Super(key, width, height), _alignCenter(alignCenter)
{
}

Sprite::~Sprite()
{
	
}

void Sprite::Render(ID2D1RenderTarget* renderTarget, Vector pos)
{
	Size frameSize = _bitmap->GetFrameSize();

	// �ҽ� ��Ʈ�ʿ��� ������ ���� ��ǥ ���
	float srcX = _indexX * (float)frameSize.Width;
	float srcY = _indexY * (float)frameSize.Height;

	// ���� ��Ʈ�ʿ��� �׸� ���� (�ҽ� ����)
	D2D1_RECT_F srcRect = D2D1::RectF(
		srcX,
		srcY,
		srcX + frameSize.Width,
		srcY + frameSize.Height);

	Vector renderPos = pos;

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

	// ��Ʈ�� ������
	renderTarget->DrawBitmap(_bitmap->GetBitmap(), destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, &srcRect);
}

Size Sprite::GetFrameSize()
{
	if (_bitmap)
	{
		return _bitmap->GetFrameSize();
	}
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
