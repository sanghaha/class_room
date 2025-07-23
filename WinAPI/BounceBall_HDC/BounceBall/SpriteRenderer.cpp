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

	// ���� �ð��� ������ ���� ������ �̵�
	if (_sumTime >= delta && _isEnd == false)
	{
		_animIndexX = ((_animIndexX + 1) % frameCountX);

		// x ������ �ѹ��� ���Ƽ� ������ ����
		if (_loop == false)
		{
			if (_animIndexX == frameCountX - 1)
			{
				// ������ �ƴ� �ִϸ��̼��� ������ �����ӿ� �����ְ� �Ѵ�.
				_isEnd = true;
			}
		}

		_sumTime -= delta;
	}
}

void SpriteRenderer::RenderComponent(HDC renderTarget, Vector pos)
{
	Size frameSize = _texture->GetFrameSize();

	// �ҽ� ��Ʈ�ʿ��� ������ ���� ��ǥ ���
	float srcX = _animIndexX * (float)frameSize.Width;
	float srcY = _animIndexY * (float)frameSize.Height;

	_texture->Render(renderTarget, pos, Vector(srcX, srcY));
}

Size SpriteRenderer::GetFrameSize()
{
	return _texture->GetFrameSize();
}
