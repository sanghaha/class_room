#pragma once

#include "DataObject.h"

class ResourceData : public DataObject
{
public:
	class Item
	{
	public:
		wstring key;
		wstring fileName;
		int32 transparent = -1;
		int32 countX = 1;
		int32 countY = 1;
		bool loop = false;
	};

public:
	virtual wstring GetFileName() override;
	virtual void Load(const json& data) override;
	virtual void Clear() override;

	const unordered_map<wstring, Item*>& GetGameSceneResource() const { return _gameSceneData; }

private:
	unordered_map<wstring, Item*> _gameSceneData;
};

