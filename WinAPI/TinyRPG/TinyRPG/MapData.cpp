#include "pch.h"
#include "MapData.h"

wstring MapData::GetFileName()
{
    return L"TileMapData.json";
}

void MapData::Load(const json& data)
{
    _tileMapPath = data["tileMapPath"];

    for (auto iter : data["canMoveTile"])
    {
        TileInfo info;
        info.minX = iter["minX"];
        info.maxX = iter["maxX"];
        info.minY = iter["minY"];
        info.maxY = iter["maxY"];
        _canMoveTile.emplace_back(info);
    }
}
