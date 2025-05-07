#pragma once


extern const int TileSize;
extern const int OriginTileSize;
extern const int GridWidth;
extern const int GridHeight;
extern const int TileMapWidth;
extern const int TileMapHeight;


struct TileInfo
{
	int32 minX = 0;
	int32 maxX = 0;
	int32 minY = 0;
	int32 maxY = 0;
};

// 레이어 2에만 배치 가능한 풀 영역
extern const TileInfo Ground;
extern const TileInfo Bridge1;
extern const TileInfo Bridge2;
extern const TileInfo Rock;
extern const TileInfo Water;

