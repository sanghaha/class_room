#include "pch.h"
#include "MapData.h"

wstring MapData::GetFileName()
{
    return L"TileMapData.json";
}

void MapData::Load(const json& data)
{
    for (auto iter : data["canMoveTile"])
    {
        TileInfo info;
        info.minX = iter["minX"];
        info.maxX = iter["maxX"];
        info.minY = iter["minY"];
        info.maxY = iter["maxY"];
        _canMoveTile.emplace_back(info);
    }

    for (auto iter : data["list"])
    {
        StageInfo info;
        info.id = iter["id"];
        info.tileMapPath = iter["tileMapPath"];
        info.monsterCount = iter["monsterCount"];
        info.startX = iter["startX"];
        info.startY = iter["startY"];
        info.linkX = iter["stage"]["linkX"];
        info.linkY = iter["stage"]["linkY"];
        info.linkMapId = iter["stage"]["linkStage"];
        _stage.emplace(info.id, info);
    }
}
