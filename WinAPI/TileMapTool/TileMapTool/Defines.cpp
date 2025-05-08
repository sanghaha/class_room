#include "pch.h"
#include "Defines.h"

const int TileSize = 64;
const int OriginTileSize = 64;	// 원본 타일 사이즈

const int GridWidth = 20;
const int GridHeight = 15;
const int TileMapWidth = 16;
const int TileMapHeight = 8;

const TileInfo Ground = TileInfo{4, 7, 0, 3};
const TileInfo Bridge1 = TileInfo{ 4, 6, 4, 4 };
const TileInfo Bridge2 = TileInfo{ 4, 4, 5, 7 };
const TileInfo Rock = TileInfo{ 0, 3, 0, 5 };
const TileInfo Water = TileInfo{ 7, 7, 4, 4 };
const TileInfo GoldMine = TileInfo{ 13, 15, 1, 2 };