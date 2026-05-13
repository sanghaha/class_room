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

void ImageRenderer::RenderComponent(HDC renderTarget, Pos pos)
{
	if (_index != -1)
	{
		Size frameSize = _texture->GetFrameSize();

		// 소스 비트맵에서 복사할 시작 좌표 계산
		float srcX = _index * (float)frameSize.w;

		_texture->Render(renderTarget, pos, Pos(srcX, 0));
	}
	else
	{
		_texture->Render(renderTarget, pos, Pos(0, 0));
	}
}

Size ImageRenderer::GetSize()
{
	return _texture->GetFrameSize();
}

void ImageRenderer::SetCenterAlign(bool center)
{
	_texture->SetCenterAlign(center);
}
