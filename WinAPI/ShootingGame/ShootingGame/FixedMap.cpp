#include "pch.h"
#include "FixedMap.h"
#include "Texture.h"

FixedMap::FixedMap(Pos pos) : Super(pos)
{
	_texture = CreateTextureComponent(L"BG_2");
	if (_texture)
	{
		// 텍스쳐의 크기를 가져온다
		_textureSize = _texture->GetSize();
	}
}

FixedMap::~FixedMap()
{
}

void FixedMap::Init()
{
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
