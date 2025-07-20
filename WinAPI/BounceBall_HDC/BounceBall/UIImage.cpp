#include "pch.h"
#include "UIImage.h"
#include "Texture.h"

UIImage::UIImage(Vector pos, wstring key, int32 width, int32 height) : Super(pos)
{
	_texture = new Texture(key, width, height);
}

UIImage::~UIImage()
{
	SAFE_DELETE(_texture);
}

void UIImage::Update()
{
}

void UIImage::Render(HDC renderTarget)
{
	if (_texture)
	{
		_texture->Render(renderTarget, _pos);
	}
}
