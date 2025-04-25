#include "pch.h"
#include "Map.h"
#include "Sprite.h"
#include <iostream>
#include <fstream>

Map::Map(Pos pos) : Super(pos)
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

void Map::Render(HDC hdc)
{
	if (_sprite == nullptr)
		return;

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
					drawTileOnGrid(hdc, i, x, y);
				}
			}
		}
	}

	//_texture->Render(hdc, GetPos());
}

void Map::drawTileOnGrid(HDC hdc, int layer, int x, int y)
{
	int tileIndex = _layer[layer].mainGrid[y * _gridW + x];

	// ������ Ÿ���� �׸��忡 �׸���
	int tileX = tileIndex % _tileCountX;
	int tileY = tileIndex / _tileCountX;

	Pos pos((float)x * _tileSize, (float)y * _tileSize);
	_sprite->Render(hdc, pos, tileX, tileY, 1);
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
	_sprite = sprite;
}

Size Map::GetMapSize()
{
	return Size(_gridW * _tileSize, _gridH * _tileSize);
}
