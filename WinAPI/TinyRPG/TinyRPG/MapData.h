#pragma once
#include "DataObject.h"

struct TileInfo
{
	int32 minX;
	int32 maxX;
	int32 minY;
	int32 maxY;
};

struct StageInfo
{
	int32 id;
	std::string tileMapPath;
	int32 monsterCount;
	int32 startX;
	int32 startY;
	int32 linkX;
	int32 linkY;
	int32 linkMapId;
};

class MapData : public DataObject
{
public:
	MapData() {}
	virtual ~MapData() {}

	wstring GetFileName() override;
	void Load(const json& data) override;


	std::vector<TileInfo> _canMoveTile;
	std::map<int32, StageInfo> _stage;
};

