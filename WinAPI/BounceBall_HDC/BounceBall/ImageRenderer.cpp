#include "pch.h"
#include "ImageRenderer.h"
#include "Game.h"
#include "ResourceManager.h"
#include "Texture.h"

ImageRenderer::ImageRenderer(wstring key, int32 width, int32 height)
{
	_texture = ResourceManager::GetInstance()->GetTexture(key);
	if (_texture == nullptr)
	{
		::MessageBox(Game::GetInstance()->GetHwnd(), key.c_str(), L"Invalid Texture Load", MB_OK);
		return;
	}

	if (width != 0 || height != 0)
	{
		_texture->SetSize(width, height);
	}
}

ImageRenderer::~ImageRenderer()
{
}

void ImageRenderer::RenderComponent(HDC renderTarget, Vector pos)
{
	_texture->Render(renderTarget, pos, Vector(0, 0));
}

Size ImageRenderer::GetSize()
{
	return _texture->GetFrameSize();
}

void ImageRenderer::SetCenterAlign(bool center)
{
	_texture->SetCenterAlign(center);
}
