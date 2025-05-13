#include "pch.h"
#include "UIImage.h"
#include "Texture.h"
#include "Sprite.h"

UIImage::UIImage(Vector pos, string key, int32 width, int32 height) : Super(pos)
{
	CreateTextureComponent(key, width, height);
}

UIImage::~UIImage()
{
}


//------------------------------------------------------------------
UISprite::UISprite(Vector pos, string spriteName, int32 width, int32 height) : Super(pos)
{
    _sprite = CreateSpriteComponent(spriteName, width, height);
}

UISprite::~UISprite()
{
}

void UISprite::SetIndex(int32 x, int32 y)
{
    if (_sprite)
    {
        _sprite->SetIndex(x, y);
    }
}

//------------------------------------------------------------------
UISliced3::UISliced3(Vector pos, string key, int32 width, int32 height, int32 left, int32 right) : Super(pos), _left(left), _right(right)
{
    _texture = CreateTextureComponent(key, width, height);
}

UISliced3::~UISliced3()
{
}

void UISliced3::Render(ID2D1RenderTarget* renderTarget)
{
    if (!_texture->GetBitmap() || _texture->GetSize().Width <= 0 || _left + _right > _texture->GetSize().Width || _ratio < 0.0f || _ratio > 1.0f)
        return;

    // 클리핑 영역 계산 (비율에 따라 시작 픽셀부터 클리핑)
    float clippedWidth = _texture->GetSize().Width * _ratio;

    // 1. Left 영역 렌더링
    float leftDrawWidth = std::min((float)_left, clippedWidth);
    if (clippedWidth > 0)
    {
        D2D1_RECT_F srcLeft = D2D1::RectF(
            0,
            0,
            static_cast<float>(leftDrawWidth),
            static_cast<float>(_texture->GetSize().Width));

        D2D1_RECT_F destLeft = D2D1::RectF(
            GetPos().x,
            GetPos().y,
            GetPos().x + static_cast<float>(leftDrawWidth),
            GetPos().y + static_cast<float>(_texture->GetSize().Height)
        );
        _texture->Render(renderTarget, srcLeft, destLeft);
    }

    // 2. Center 영역 렌더링
    float centerAvailableWidth = clippedWidth - _left;
    float centerWidth = (float)_texture->GetSize().Width - _left - _right;
    float centerDrawWidth = min(centerWidth, centerAvailableWidth);
    if (centerAvailableWidth < 0)
        return;

    {
        D2D1_RECT_F srcCenter = D2D1::RectF(
            static_cast<float>(_left),
            0,
            static_cast<float>(_texture->GetSize().Width - _right),
            static_cast<float>(_texture->GetSize().Height)
        );
        D2D1_RECT_F destCenter = D2D1::RectF(
            GetPos().x + static_cast<float>(_left),
            GetPos().y,
            GetPos().x + static_cast<float>(_left + centerDrawWidth),
            GetPos().y + static_cast<float>(_texture->GetSize().Height)
        );
        _texture->Render(renderTarget, srcCenter, destCenter);
    }

    // 3. Right 영역 렌더링
    float rightClipStart = clippedWidth - leftDrawWidth - centerDrawWidth;
    if (rightClipStart > 0)
    {
        float drawWidth = min((float)_right, rightClipStart);
        D2D1_RECT_F srcRight = D2D1::RectF(
            static_cast<float>(_texture->GetSize().Width - _right),
            0,
            static_cast<float>(_texture->GetSize().Width),
            static_cast<float>(_texture->GetSize().Height)
        );
        D2D1_RECT_F destRight = D2D1::RectF(
            GetPos().x + static_cast<float>(_texture->GetSize().Width - _right),
            GetPos().y,
            GetPos().x + static_cast<float>(_texture->GetSize().Width - _right + rightClipStart),
            GetPos().y + static_cast<float>(_texture->GetSize().Height)
        );
        _texture->Render(renderTarget, srcRight, destRight);
    }
}


//------------------------------------------------------------------------------------------------
UINumber::UINumber(Vector pos, string key, int32 width, int32 height) : Super(pos)
{
    _sprite = CreateSpriteComponent(key, width, height);
}

UINumber::~UINumber()
{
}

void UINumber::Render(ID2D1RenderTarget* renderTarget)
{
    if (_sprite == nullptr)
        return;

    // 123 
    // 3 -> 123 % 10 = 3
    // 2 -> (123 / 10) % 10 
    // 1 -> (123 / 100) % 10

    // 문자열 변환
    // string str = std::to_string(number);
    int32 tempNumber = _number;
    for (int32 i = 0; i < _numberPos.size(); ++i)
    {
        int32 number = tempNumber % 10;
        tempNumber = tempNumber / ((i + 1) * 10);

        _sprite->SetIndex(number, 0);
        _sprite->Render(renderTarget, GetPos() + _numberPos[i]);
    }
}

void UINumber::SetNumber(int8 number)
{
    if (_sprite == nullptr)
        return;

    // 숫자 10단위마다 위치값을 조절한다
    if (_number != number)
    {
        int32 count = number / 10;
        _numberPos.clear();
        for (int32 i = count; i >= 0; i--)
        {
            _numberPos.push_back(Vector(i * (float)_sprite->GetSize().Width, 0));
        }
    }

    _number = number;
}

