#include "pch.h"
#include "Map.h"
#include "Texture.h"


Map::Map(Pos pos) : Super(pos)
{
	_texture = CreateTextureComponent(L"BG");
	if (_texture)
	{
		// �ؽ����� ũ�⸦ �����´�
		Size size = _texture->GetSize();
		_textureHeight = size.h;
		_pos2 = { 0, (float)(-_textureHeight) };
	}
}

Map::~Map()
{
}

void Map::Init()
{
	Super::Init();
}

void Map::Update(float deltaTime)
{
	AddPosDelta(0, (_moveSpeed * deltaTime), false);
	_pos2.y += (_moveSpeed * deltaTime);

	if (GetPos().y >= _textureHeight)
	{
		AddPosDelta(0, -_textureHeight * 2.0f, false);
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

	_texture->Render(hdc, GetPos());
	_texture->Render(hdc, _pos2);
}
