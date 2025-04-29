#pragma once
#include "Singleton.h"
#include "DataObject.h"

class DataObject;
class MonsterData;

class DataManager : public Singleton<DataManager>
{
public:
	DataManager();
	virtual ~DataManager();

	void Init();
	void Destroy() override;

	template<typename T>
	const T* GetData(wstring key) const
	{
		auto find = _data.find(key);
		if (find != _data.end())
		{
			// �̹� �����ϴ� Ű��� ����
			return dynamic_cast<T*>(find->second);
		}

		return nullptr;
	}

	const MonsterData* GetMonsterData(int32 key) const
	{
		auto find = _monsterData.find(key);
		if (find != _monsterData.end())
		{
			// �̹� �����ϴ� Ű��� ����
			return find->second;
		}

		return nullptr;
	}

private:
	void loadDataObject(fs::path directory, wstring key, DataObject* obj);
	void loadMonsterDataObject(fs::path directory);

private:
	map<wstring, DataObject*>  _data;
	map<int32, MonsterData*>  _monsterData;
};

