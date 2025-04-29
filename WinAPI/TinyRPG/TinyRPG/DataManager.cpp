#include "pch.h"
#include "DataManager.h"
#include "ResourceManager.h"
#include "MapData.h"
#include "MonsterData.h"

DataManager::DataManager()
{
}

DataManager::~DataManager()
{
}

void DataManager::Init()
{
    fs::path directory = ResourceManager::GetInstance()->GetResourcePath() / L"Data/";

    // map 
    {
        MapData* mapData = new MapData();
        loadDataObject(directory, L"MapData", mapData);
    }

    // monster
    {
        loadMonsterDataObject(directory);
    }
}

void DataManager::Destroy()
{
    for (auto& [key, res] : _data)
    {
        delete res;
    }
    _data.clear();
}

void DataManager::loadDataObject(fs::path directory, wstring key, DataObject* obj)
{
    if (_data.find(key) != _data.end())
    {
        // �̹� �����ϴ� Ű��� ����
        return;
    }

    fs::path path = directory / obj->GetFileName();
    std::ifstream file(path.c_str());
    if (!file.is_open())
    {
        MessageBox(nullptr, L"Failed to open JSON file", L"Error", MB_OK);
        return;
    }

    json data = json::parse(file);
    obj->Load(data);

    _data.emplace(key, obj);
}

void DataManager::loadMonsterDataObject(fs::path directory)
{
    fs::path path = directory / L"Monster.json";
    std::ifstream file(path.c_str());
    if (!file.is_open())
    {
        MessageBox(nullptr, L"Failed to open JSON file", L"Error", MB_OK);
        return;
    }

    json data = json::parse(file);
    for (auto iter : data["list"])
    {
        int32 id = iter["id"];
        if (id == 0)
            continue;

        MonsterData* data = new MonsterData();
        data->Load(iter);
        _monsterData.emplace(id, data);
    }
}
