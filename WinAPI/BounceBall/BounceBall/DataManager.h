#pragma once
#include "Singleton.h"

class DataObject;

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
			// 이미 존재하는 키라면 리턴
			return dynamic_cast<T*>(find->second);
		}

		return nullptr;
	}


private:


private:
	map<wstring, DataObject*>	_data;
};

