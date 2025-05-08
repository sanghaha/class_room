#include "pch.h"
#include "InventorySystem.h"
#include "Item.h"
#include "ItemData.h"
#include "DataManager.h"

InventorySystem::InventorySystem()
{
	//memset(_equips, 0, sizeof(_equips));
	//memset(_slots, 0, sizeof(_slots));
}

InventorySystem::~InventorySystem()
{
}

void InventorySystem::Init()
{
	// 기본 검장착
	auto itemData = DataManager::GetInstance()->GetItemData(401);
	AddItem(itemData);
	EquipItem(0);
}

void InventorySystem::Destroy()
{
	for (int32 i = 0; i < MAX_INVEN_SLOT; ++i)
	{
		SAFE_DELETE(_slots[i]);
		_slots[i] = nullptr;
	}
}

// 아이템 장착 처리
void InventorySystem::SceneStart()
{
	for (auto item : _equips)
	{
		if (item)
		{
			item->EquipItem();
		}
	}
}

bool InventorySystem::AddItem(const ItemData* data)
{
	int32 emptySlotIdx = getEmptySlotIdx();
	if (emptySlotIdx == -1)
		return false;

	InvenItem* item = new InvenItem(data);
	_slots[emptySlotIdx] = item;
	return true;
}

void InventorySystem::UseItem(int32 slotIdx)
{
	if (slotIdx < 0 || slotIdx >= MAX_INVEN_SLOT)
		return;

	// 소모품 아이템
	if (_slots[slotIdx]->GetItemSlotType() == ItemSlot::Grocery)
	{
		_slots[slotIdx]->UseItem();

		// 아이템 삭제
		SAFE_DELETE(_slots[slotIdx]);
		_slots[slotIdx] = nullptr;
	}
	else
	{
		EquipItem(slotIdx);
	}
}

InvenItem* InventorySystem::GetItem(int32 slotIdx)
{
	if (slotIdx < 0 || slotIdx >= MAX_INVEN_SLOT)
		return nullptr;

	return _slots[slotIdx];
}

bool InventorySystem::EquipItem(int32 invenSlotIdx)
{
	InvenItem* invenItem = GetItem(invenSlotIdx);
	if (nullptr == invenItem)
		return false;

	ItemSlot slotType = invenItem->GetItemSlotType();
	if (slotType == ItemSlot::NoneSlot)
		return false;

	// 슬롯 바꾸기
	InvenItem* prevEquipItem = _equips[slotType];
	_slots[invenSlotIdx] = prevEquipItem;
	_equips[slotType] = invenItem;

	if (prevEquipItem) prevEquipItem->UnequipItem();
	invenItem->EquipItem();
	return true;
}

InvenItem* InventorySystem::GetEqiupItem(ItemSlot slot)
{
	if(slot < 0 || slot >= MAX_EQUIP_SLOT)
		return nullptr;

	return _equips[slot];
}

bool InventorySystem::UnequipItem(ItemSlot equipSlotIdx)
{
	InvenItem* prevEquipItem = GetEqiupItem(equipSlotIdx);
	if(nullptr == prevEquipItem)
		return false;

	int32 emptySlotIdx = getEmptySlotIdx();
	if (emptySlotIdx == -1)
		return false;

	_slots[emptySlotIdx] = prevEquipItem;
	_equips[equipSlotIdx] = nullptr;

	prevEquipItem->UnequipItem();
	return true;
}

bool InventorySystem::IsFull() const
{
	return getEmptySlotIdx() == -1;
}

int32 InventorySystem::getEmptySlotIdx() const
{
	for(int32 i = 0; i < MAX_INVEN_SLOT; ++i)
	{
		if (_slots[i] == nullptr)
			return i;
	}
	return -1;
}
