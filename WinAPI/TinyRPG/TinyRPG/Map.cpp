#include "pch.h"
#include "Map.h"
#include "Sprite.h"
#include <iostream>
#include <fstream>
#include "Game.h"
#include "Scene.h"

Map::Map(Vector pos) : Super(pos)
{
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
}

void Map::Render(ID2D1HwndRenderTarget* renderTarget)
{
	// Ÿ�� �׸���
	for (int i = 0; i < _layerCount; i++)
	{
		// ���̾ Ÿ�� �׸���
		for (int y = 0; y < _gridH; y++) 
		{
			for (int x = 0; x < _gridW; x++) 
			{
				int tileIndex = _layer[i].mainGrid[y * _gridW + x];
				if (tileIndex >= 0) 
				{
					drawTileOnGrid(renderTarget, i, x, y);
				}
			}
		}
	}

	//_texture->Render(hdc, GetPos());
}

void Map::drawTileOnGrid(ID2D1HwndRenderTarget* renderTarget, int layer, int x, int y)
{
	int tileIndex = _layer[layer].mainGrid[y * _gridW + x];

	// ������ Ÿ���� �׸��忡 �׸���
	int tileX = tileIndex % _tileCountX;
	int tileY = tileIndex / _tileCountX;

	Vector pos((float)x * GTileSize, (float)y * GTileSize);
	if (Game::GetScene()->IsCulling(pos))
		return;
	
	_spriteRenderer.SetIndex(tileX, tileY);
	_spriteRenderer.Render(renderTarget, pos);
}

void Map::LoadTileMap(wstring path)
{
	std::wifstream file(path);
	if (!file.is_open())
		return;

	wchar_t comma;

	// �׸��� ũ�� �ε�
	int width, height, tileX, tileY;
	file >> width >> comma >> height >> comma >> tileX >> comma >> tileY;

	_gridW = width;
	_gridH = height;
	_tileCountX = tileX;
	_tileCountY = tileY;

	// ���̾� ������ �ʱ�ȭ
	for (int i = 0; i < _layerCount; i++) {
		_layer[i].mainGrid.assign(width * height, -1);
	}

	// ���̾ ������ �ε�
	std::wstring line;
	while (std::getline(file, line)) 
	{
		std::wistringstream iss(line);

		// ���̾� ��ȣ Ȯ��
		int layerIndex, tileCount;
		if (iss >> layerIndex >> comma >> tileCount)
		{
			// Ÿ�� ������ �б�
			for (int index = 0; index < tileCount; ++index)
			{
				std::getline(file, line);

				if (line.empty() == false)
				{
					std::wistringstream tileStream(line);
					int x, y, tileIndex;
					wchar_t comma;

					if (tileStream >> x >> comma >> y >> comma >> tileIndex) 
					{
						if (x >= 0 && x < width && y >= 0 && y < height) 
						{
							_layer[layerIndex].mainGrid[y * width + x] = tileIndex;
						}
					}
				}
			}
		}
	}
}

void Map::SetSprite(Sprite* sprite)
{
	SpriteRenderInfo info;
	info.alignCenter = false;
	_spriteRenderer.SetSprite(sprite, info);
}

Size Map::GetMapSize()
{
	return Size(_gridW * GTileSize, _gridH * GTileSize);
}

void Map::ConvertTopTileIndex(int32 x, int32 y, int32& outTileX, int32& outTileY)
{
	int32 index = y * _gridW + x;
	if (index < 0 || index >= _layer[_layerCount - 1].mainGrid.size())
		return;

	int32 tileIndex = _layer[_layerCount-1].mainGrid[y * _gridW + x];

	outTileX = tileIndex % _tileCountX;
	outTileY = tileIndex / _tileCountX;
}

