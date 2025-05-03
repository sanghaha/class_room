#include "pch.h"
#include "Texture.h"
#include "Game.h"

/// <summary>
/// UI 용 텍스쳐
/// </summary>
Sliced3Texture::Sliced3Texture()
{
}

Sliced3Texture::~Sliced3Texture()
{
}

void Sliced3Texture::Load(wstring path, int32 left, int32 right)
{
	loadBitmap(path);

	// 이미지 크기 가져오기
	_left = left;
	_right = right;
}

void Sliced3Texture::Render(ID2D1HwndRenderTarget* renderTarget, Vector pos, int32 sizeX, int32 sizeY, float clipRatio)
{
	if (!_bitmap || sizeX <= 0 || _left + _right > sizeX || clipRatio < 0.0f || clipRatio > 1.0f)
		return;

    // 클리핑 영역 계산 (비율에 따라 시작 픽셀부터 클리핑)
    float clippedWidth = sizeX * clipRatio;

    // 1. Left 영역 렌더링
    float leftDrawWidth = min(_left, clippedWidth);
    if (clippedWidth > 0)
    {
        D2D1_RECT_F srcLeft = D2D1::RectF(
            0, 
            0, 
            static_cast<float>(leftDrawWidth),
            static_cast<float>(_sizeY));

        D2D1_RECT_F destLeft = D2D1::RectF(
            pos.x,
            pos.y,
            pos.x + static_cast<float>(leftDrawWidth),
            pos.y + static_cast<float>(sizeY)
        );
        renderTarget->DrawBitmap(_bitmap, destLeft, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &srcLeft);
    }
    
    // 2. Center 영역 렌더링
    float centerAvailableWidth = clippedWidth - _left;
    float centerWidth = (float)sizeX - _left - _right;
    float centerDrawWidth = min(centerWidth, centerAvailableWidth);
    if (centerAvailableWidth < 0)
        return;

    {
		D2D1_RECT_F srcCenter = D2D1::RectF(
			static_cast<float>(_left),
			0,
			static_cast<float>(_sizeX - _right),
			static_cast<float>(_sizeY)
		);
		D2D1_RECT_F destCenter = D2D1::RectF(
			pos.x + static_cast<float>(_left),
			pos.y,
			pos.x + static_cast<float>(_left + centerDrawWidth),
			pos.y + static_cast<float>(sizeY)
		);
		renderTarget->DrawBitmap(_bitmap, destCenter, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &srcCenter);
	}

    // 3. Right 영역 렌더링
    float rightClipStart = clippedWidth - leftDrawWidth - centerDrawWidth;
    if(rightClipStart > 0)
    {
        float drawWidth = min(_right, rightClipStart);
        D2D1_RECT_F srcRight = D2D1::RectF(
            static_cast<float>(_sizeX - _right),
            0,
            static_cast<float>(_sizeX),
            static_cast<float>(_sizeY)
        );
        D2D1_RECT_F destRight = D2D1::RectF(
            pos.x + static_cast<float>(sizeX - _right),
            pos.y,
            pos.x + static_cast<float>(sizeX - _right + rightClipStart),
            pos.y + static_cast<float>(sizeY)
        );
       renderTarget->DrawBitmap(_bitmap, destRight, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &srcRight);
    }
}

PNGTexture::PNGTexture()
{
}

PNGTexture::~PNGTexture()
{
}

void PNGTexture::Render(ID2D1HwndRenderTarget* renderTarget, Vector pos)
{
    D2D1_RECT_F srcLeft = D2D1::RectF(
        0,
        0,
        static_cast<float>(_sizeX),
        static_cast<float>(_sizeY));

    D2D1_RECT_F destLeft = D2D1::RectF(
        pos.x,
        pos.y,
        pos.x + static_cast<float>(_width),
        pos.y + static_cast<float>(_height)
    );
    renderTarget->DrawBitmap(_bitmap, destLeft, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &srcLeft);
}

void PNGTexture::Load(wstring path, int32 width, int32 height)
{
    loadBitmap(path);

    _width = width != 0 ? width : _sizeX;
    _height = height != 0 ? height : _sizeY;
}
