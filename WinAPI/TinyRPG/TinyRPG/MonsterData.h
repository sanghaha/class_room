#pragma once
#include "DataObject.h"

class MonsterData : public DataObject
{
public:
	MonsterData() {}
	virtual ~MonsterData() {}

	wstring GetFileName() override;
	void Load(const json& data) override;

	string _name;
	string _spriteName;
	int32 _hp = 0;
	int32 _walkSpeed = 0;
	int32 _runSpeed = 0;
	int32 _attack = 0;
};

