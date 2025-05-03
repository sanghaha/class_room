#include "pch.h"
#include "ItemData.h"

const char* ItemEffectTypeStrings[] = {
	FOREACH_ITEM_EFFECT_TYPE(GENERATE_STRING)
};

wstring ItemData::GetFileName()
{
	return L"ItemData.json";
}

// 매크로 사용
ItemEffectType convertEffectType(string str)
{
	for (int32 i = 0; i < ItemEffectType::Max; ++i)
	{
		if (str.compare(ItemEffectTypeStrings[i]) == 0)
		{
			return (ItemEffectType)i;
		}
	}

	return ItemEffectType::None;
}

void ItemData::Load(const json& data)
{
	_id = data["id"];
	_name = data["Name"];
	_spriteName = data["SpriteName"];
	_desc = data["Desc"];
	_effectType = convertEffectType(data["EffectType"]);
	ConvertEnum<ItemSlot>(data, "Slot", _equipSlot);
	ConvertEnum<WeaponType>(data, "WeaponType", _weaponType);
	ConvertEnum<StatType>(data, "stat", _statType);
	/*
	{
		
		string equip_name = data["Slot"];
		auto equipSlot = magic_enum::enum_cast<ItemSlot>(equip_name);
		if (equipSlot.has_value())
		{
			_equipSlot = equipSlot.value();
		}
	}
	{
		string stat_name = data["stat"];
		auto statType = magic_enum::enum_cast<StatType>(stat_name);
		if (statType.has_value())
		{
			_statType = statType.value();
		}
	}
	*/
	_value = data["value"];
}
