#include "pch.h"
#include "DataManager.h"
#include "ResourceManager.h"
#include "DataObject.h"

DataManager::DataManager()
{
}

DataManager::~DataManager()
{
}

void DataManager::Init()
{
    fs::path directory = ResourceManager::GetInstance()->GetResourcePath() / L"Data/";

}

void DataManager::Destroy()
{
    for (auto& [key, res] : _data)
    {
        delete res;
    }
    _data.clear();
}