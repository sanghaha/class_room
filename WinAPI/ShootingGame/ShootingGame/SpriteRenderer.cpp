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
	float frameTime = _durtaion / totalCount;

	// 일정 시간이 지나면 다음 프레임 이동
	if (_sumTime >= frameTime)
	{
		_sumTime -= frameTime;

		// 현재 인덱스를 선형적으로 계산 (fullFrame 여부에 따라 범위가 달라짐)
		int32 currentIndex = (_animIndexY * frameCountX + _animIndexX);
		int32 nextIndex = currentIndex + 1;

		if (nextIndex >= totalCount)
		{
			if (_loop)
			{
				nextIndex = 0;
			}
			else
			{
				_isEnd = true;
				return; // 마지막 프레임 유지
			}
		}

		// 인덱스 업데이트
		_animIndexX = nextIndex % frameCountX;
		_animIndexY = nextIndex / frameCountX;
	}
}

void SpriteRenderer::RenderComponent(HDC renderTarget, Pos pos)
{
	Size frameSize = _texture->GetFrameSize();

	// 소스 비트맵에서 복사할 시작 좌표 계산
	float srcX = _animIndexX * (float)frameSize.w;
	float srcY = _animIndexY * (float)frameSize.h;

	_texture->Render(renderTarget, pos, Pos(srcX, srcY));
}

Size SpriteRenderer::GetFrameSize()
{
	return _texture->GetFrameSize();
}
