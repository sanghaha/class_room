#include "pch.h"
#include "Map.h"
#include "Sprite.h"
#include <iostream>
#include <fstream>
#include "Game.h"
#include "Scene.h"
#include "InputManager.h"
#include "Texture.h"
#include "ResourceManager.h"

Map::Map(Vector pos) : Super(pos)
{
	_tile = CreateSpriteComponent("TileMap");
	_selector = CreateTextureComponent("TileSelector", GTileSize, GTileSize);
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

void Map::Render(ID2D1RenderTarget* renderTarget)
{
	// 타일 그리기
	for (int i = 0; i < _layerCount; i++)
	{
		// 레이어별 타일 그리기
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
}

void Map::drawTileOnGrid(ID2D1RenderTarget* renderTarget, int layer, int x, int y)
{
	int tileIndex = _layer[layer].mainGrid[y * _gridW + x];

	// 선택한 타일을 그리드에 그리기
	int tileX = tileIndex % _tileCountX;
	int tileY = tileIndex / _tileCountX;

	Vector pos((float)x * GTileSize, (float)y * GTileSize);
	if (Game::GetScene()->IsCulling(pos))
		return;
	
	if (_tile)
	{
		_tile->SetIndex(tileX, tileY);
		_tile->Render(renderTarget, pos);
	}

	if (x == _selectedX && y == _selectedY && _selector)
	{
		Vector screenPos = Game::ConvertScreenPos(pos);
		_selector->Render(renderTarget, screenPos);
	}
}

void Map::LoadTileMap(int8 stage, wstring path)
{
	std::wifstream file(path);
	if (!file.is_open())
		return;

	wchar_t comma;

	// 그리드 크기 로드
	int width, height, tileX, tileY;
	file >> width >> comma >> height >> comma >> tileX >> comma >> tileY;

	_gridW = width;
	_gridH = height;
	_tileCountX = tileX;
	_tileCountY = tileY;

	// 레이어 데이터 초기화
	for (int i = 0; i < _layerCount; i++) {
		_layer[i].mainGrid.assign(width * height, -1);
	}

	// 레이어별 데이터 로드
	std::wstring line;
	while (std::getline(file, line)) 
	{
		std::wistringstream iss(line);

		// 레이어 번호 확인
		int layerIndex, tileCount;
		if (iss >> layerIndex >> comma >> tileCount)
		{
			// 타일 데이터 읽기
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

Size Map::GetMapSize()
{
	return Size(_gridW * GTileSize, _gridH * GTileSize);
}

void Map::ConvertGroundTileIndex(int32 x, int32 y, int32& outTileX, int32& outTileY)
{
	int32 groundLayer = 3;
	int32 index = y * _gridW + x;
	if (index < 0 || index >= _layer[groundLayer - 1].mainGrid.size())
		return;

	int32 tileIndex = _layer[groundLayer -1].mainGrid[y * _gridW + x];

	outTileX = tileIndex % _tileCountX;
	outTileY = tileIndex / _tileCountX;
}

