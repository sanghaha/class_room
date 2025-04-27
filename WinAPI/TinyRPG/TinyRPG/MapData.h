#pragma once
#include "DataObject.h"

struct TileInfo
{
	int32 minX;
	int32 maxX;
	int32 minY;
	int32 maxY;
};

class MapData : public DataObject
{
public:
	MapData() {}
	virtual ~MapData() {}

	wstring GetFileName() override;
	void Load(const json& data) override;

	std::string _tileMapPath;
	std::vector<TileInfo> _canMoveTile;
	int32 _monsterCount;
};

