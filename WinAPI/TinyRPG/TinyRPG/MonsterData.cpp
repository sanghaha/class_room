#include "pch.h"
#include "MonsterData.h"

wstring MonsterData::GetFileName()
{
    return L"Monster.json";
}

void MonsterData::Load(const json& data)
{
	_name = data["Name"];
	_spriteName = data["SpriteName"];
	_hp = data["Hp"];
	_walkSpeed = data["WalkSpeed"];
	_runSpeed = data["RunSpeed"];
	_attack = data["Attack"];
}
