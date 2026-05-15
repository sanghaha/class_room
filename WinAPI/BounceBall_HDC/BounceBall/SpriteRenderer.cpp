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
	if (_isEnd || _texture == nullptr || _durtaion <= 0)
		return;

	_sumTime += deltaTime;

	int32 frameCountX, frameCountY;
	_texture->GetFrameCount(frameCountX, frameCountY);

	int32 totalCount = _fullFrame ? (frameCountX * frameCountY) : frameCountX;
	if (totalCount <= 0)
		return;

	float frameTime = _durtaion / totalCount;

	if (_sumTime >= frameTime)
	{
		_sumTime -= frameTime;

		// 현재 인덱스를 선형적으로 계산 (fullFrame 여부에 따라 범위가 달라짐)
		int32 currentIndex = _fullFrame ? (_animIndexY * frameCountX + _animIndexX) : _animIndexX;
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
		if (_fullFrame)
			_animIndexY = nextIndex / frameCountX;
	}
}

void SpriteRenderer::RenderComponent(HDC renderTarget, Vector pos)
{
	Size frameSize = _texture->GetFrameSize();

	// 루프가 아닌 애니메이션은 마지막 프레임에 멈춰있게 한다.
	float srcX = _animIndexX * (float)frameSize.Width;
	float srcY = _animIndexY * (float)frameSize.Height;

	_texture->Render(renderTarget, pos, Vector(srcX, srcY));
}

Size SpriteRenderer::GetFrameSize()
{
	return _texture->GetFrameSize();
}

void SpriteRenderer::ResetAnim(int32 row, bool loop, float duration)
{
	_animIndexX = 0;
	_animIndexY = row;
	_isEnd = false;
	_sumTime = 0;
	_loop = loop;
	_durtaion = duration;
}
