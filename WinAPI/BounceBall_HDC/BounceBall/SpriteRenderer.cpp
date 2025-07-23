#include "pch.h"
#include "SpriteRenderer.h"
#include "Game.h"
#include "ResourceManager.h"
#include "Texture.h"

SpriteRenderer::SpriteRenderer(wstring key, int32 width, int32 height)
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

SpriteRenderer::~SpriteRenderer()
{
	
}

void SpriteRenderer::UpdateComponent(float deltaTime)
{
	if (_isEnd)
		return;

	if (_texture == nullptr)
		return;

	if (_durtaion <= 0)
		return;

	_sumTime += deltaTime;

	int32 frameCountX = 0;
	int32 frameCountY = 0;
	_texture->GetFrameCount(frameCountX, frameCountY);

	int32 totalCount = frameCountX * frameCountY;
	float delta = _durtaion / totalCount;

	// 일정 시간이 지나면 다음 프레임 이동
	if (_sumTime >= delta && _isEnd == false)
	{
		_animIndexX = ((_animIndexX + 1) % frameCountX);

		// x 개수가 한바퀴 돌아서 끝까지 도착
		if (_loop == false)
		{
			if (_animIndexX == frameCountX - 1)
			{
				// 루프가 아닌 애니메이션은 마지막 프레임에 멈춰있게 한다.
				_isEnd = true;
			}
		}

		_sumTime -= delta;
	}
}

void SpriteRenderer::RenderComponent(HDC renderTarget, Vector pos)
{
	Size frameSize = _texture->GetFrameSize();

	// 소스 비트맵에서 복사할 시작 좌표 계산
	float srcX = _animIndexX * (float)frameSize.Width;
	float srcY = _animIndexY * (float)frameSize.Height;

	_texture->Render(renderTarget, pos, Vector(srcX, srcY));
}

Size SpriteRenderer::GetFrameSize()
{
	return _texture->GetFrameSize();
}
