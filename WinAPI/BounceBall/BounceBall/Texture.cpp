#include "pch.h"
#include "Texture.h"
#include "Game.h"
#include "ResourceManager.h"
#include "DXBitmap.h"

/// <summary>
/// UI 용 텍스쳐
/// </summary>
Sliced3Texture::Sliced3Texture(wstring key, int32 width, int32 height, int32 left, int32 right) : Super(key, width, height)
{
    // 이미지 크기 가져오기
    _left = left;
    _right = right;
}

Sliced3Texture::~Sliced3Texture()
{
}

void Sliced3Texture::Render(ID2D1RenderTarget* renderTarget, Vector pos)
{
	if (!_bitmap->GetBitmap() || _size.Width <= 0 || _left + _right > _size.Width || _ratio < 0.0f || _ratio > 1.0f)
		return;

    // 클리핑 영역 계산 (비율에 따라 시작 픽셀부터 클리핑)
    float clippedWidth = _size.Width * _ratio;

    // 1. Left 영역 렌더링
    float leftDrawWidth = std::min((float)_left, clippedWidth);
    if (clippedWidth > 0)
    {
        D2D1_RECT_F srcLeft = D2D1::RectF(
            0, 
            0, 
            static_cast<float>(leftDrawWidth),
            static_cast<float>(_bitmap->GetBitmapSize().Width));

        D2D1_RECT_F destLeft = D2D1::RectF(
            pos.x,
            pos.y,
            pos.x + static_cast<float>(leftDrawWidth),
            pos.y + static_cast<float>(_size.Height)
        );
        renderTarget->DrawBitmap(_bitmap->GetBitmap(), destLeft, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &srcLeft);
    }
    
    // 2. Center 영역 렌더링
    float centerAvailableWidth = clippedWidth - _left;
    float centerWidth = (float)_size.Width - _left - _right;
    float centerDrawWidth = min(centerWidth, centerAvailableWidth);
    if (centerAvailableWidth < 0)
        return;

    {
		D2D1_RECT_F srcCenter = D2D1::RectF(
			static_cast<float>(_left),
			0,
			static_cast<float>(_bitmap->GetBitmapSize().Width - _right),
			static_cast<float>(_bitmap->GetBitmapSize().Height)
		);
		D2D1_RECT_F destCenter = D2D1::RectF(
			pos.x + static_cast<float>(_left),
			pos.y,
			pos.x + static_cast<float>(_left + centerDrawWidth),
			pos.y + static_cast<float>(_size.Height)
		);
		renderTarget->DrawBitmap(_bitmap->GetBitmap(), destCenter, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &srcCenter);
	}

    // 3. Right 영역 렌더링
    float rightClipStart = clippedWidth - leftDrawWidth - centerDrawWidth;
    if(rightClipStart > 0)
    {
        float drawWidth = min((float)_right, rightClipStart);
        D2D1_RECT_F srcRight = D2D1::RectF(
            static_cast<float>(_bitmap->GetBitmapSize().Width - _right),
            0,
            static_cast<float>(_bitmap->GetBitmapSize().Width),
            static_cast<float>(_bitmap->GetBitmapSize().Height)
        );
        D2D1_RECT_F destRight = D2D1::RectF(
            pos.x + static_cast<float>(_size.Width - _right),
            pos.y,
            pos.x + static_cast<float>(_size.Width - _right + rightClipStart),
            pos.y + static_cast<float>(_size.Height)
        );
       renderTarget->DrawBitmap(_bitmap->GetBitmap(), destRight, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &srcRight);
    }
}

Texture::Texture(wstring key, int32 width, int32 height) : Super(key, width, height)
{
}

Texture::~Texture()
{
}

void Texture::Render(ID2D1RenderTarget* renderTarget, Vector pos)
{
    if (_bitmap->GetBitmap() == nullptr)
        return;

    D2D1_RECT_F srcLeft = D2D1::RectF(
        0,
        0,
        static_cast<float>(_bitmap->GetBitmapSize().Width),
        static_cast<float>(_bitmap->GetBitmapSize().Height));

    D2D1_RECT_F destLeft = D2D1::RectF(
        pos.x,
        pos.y,
        pos.x + static_cast<float>(_size.Width),
        pos.y + static_cast<float>(_size.Height)
    );
    renderTarget->DrawBitmap(_bitmap->GetBitmap(), destLeft, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &srcLeft);
}