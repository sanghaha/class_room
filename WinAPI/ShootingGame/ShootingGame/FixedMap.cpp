#include "pch.h"
#include "FixedMap.h"
#include "Texture.h"
#include "ImageRenderer.h"

FixedMap::FixedMap()
{
}

FixedMap::~FixedMap()
{
}

void FixedMap::Init(Pos pos)
{
	SetPos(pos, false);

	if (_texture == nullptr)
	{
		_texture = CreateTextureComponent(L"BG_2");
		if (_texture)
		{
			// 텍스쳐의 크기를 가져온다
			_textureSize = _texture->GetSize();
		}
	}

	Super::Init();
}

void FixedMap::Update(float deltaTime)
{
	Super::Update(deltaTime);
}

void FixedMap::Render(HDC hdc)
{
	Super::Render(hdc);
}

Size FixedMap::GetMapSize()
{
	return _textureSize;
}
