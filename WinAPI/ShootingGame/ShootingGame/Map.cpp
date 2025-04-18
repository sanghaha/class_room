#include "pch.h"
#include "Map.h"
#include "Texture.h"

Map::Map(Pos pos) : Super(pos)
{
}

Map::~Map()
{
}

void Map::Init()
{
}

void Map::Update(float deltaTime)
{
	_pos.y += (_moveSpeed * deltaTime);
	_pos2.y += (_moveSpeed * deltaTime);

	if (_pos.y >= _textureHeight)
	{
		_pos.y -= (_textureHeight * 2);
	}

	if (_pos2.y >= _textureHeight)
	{
		_pos2.y -= (_textureHeight * 2);
	}

	//wstring text = std::format(L"Udpate : {0}, {1}\n", _pos.x, _pos.y);
	//OutputDebugString(text.c_str());
}

void Map::Render(HDC hdc)
{
	if (_texture == nullptr)
		return;

	_texture->Render(hdc, _pos);
	_texture->Render(hdc, _pos2);
}

void Map::SetTexture(Texture* texture)
{
	_texture = texture;

	// 텍스쳐의 크기를 가져온다
	Size size = texture->GetSize();
	_textureHeight = size.h;
	_pos2 = { 0, (float)(-_textureHeight)};
}
