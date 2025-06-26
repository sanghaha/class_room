#include "pch.h"
#include "ResourceData.h"

wstring ResourceData::GetFileName()
{
    return L"ResourceData.json";
}

void ResourceData::Load(const json& data)
{
    auto gameSceneData = data["GameScene"];

    for (json::iterator it = gameSceneData.begin(); it != gameSceneData.end(); ++it)
    {
        auto key = it.key();	// 1000
        auto value = it.value(); // { }

        ResourceData::Item* item = new ResourceData::Item();
        item->key = Utf8ToWide(key);
        item->fileName = Utf8ToWide(value["fileName"]);
        
        if (value.contains("countX"))
        {
            item->countX = value["countX"];
        }

        if (value.contains("countY"))
        {
            item->countY = value["countY"];
        }

        if (value.contains("loop"))
        {
            item->loop = value["loop"];
        }

        if (value.contains("transparent"))
        {
            item->transparent = RGB(value["transparent"][0], value["transparent"][1], value["transparent"][2]);
        }
        _gameSceneData.insert(make_pair(item->key, item));
    }
}

void ResourceData::Clear()
{
    for (auto iter : _gameSceneData)
    {
        delete iter.second;
    }
    _gameSceneData.clear();
}
