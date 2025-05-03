#pragma once
#include "DataObject.h"

#define FOREACH_ITEM_EFFECT_TYPE(ITEM)  \
    ITEM(None)                \
    ITEM(ChangeStat)          \
    ITEM(Equip)               \
    ITEM(Max)

#define GENERATE_ENUM(name) name,
#define GENERATE_STRING(name) #name,

enum ItemEffectType
{
    FOREACH_ITEM_EFFECT_TYPE(GENERATE_ENUM)
};

enum StatType
{
    NoType,
    Attack,
    Defense,
    HP,
};

enum ItemSlot
{
    NoneSlot,
    Weapon,
    Armor,
    Grocery,
};

class ItemData : public DataObject
{
public:
	ItemData() {}
	virtual ~ItemData() {}

	wstring GetFileName() override;
	void Load(const json& data) override;

    int32 _id = 0;
    string _name;
    string _spriteName;
    string _desc;
    ItemSlot _equipSlot;
    ItemEffectType _effectType = ItemEffectType::None;
    StatType _statType = StatType::NoType;
    int32 _value = 0;
};

