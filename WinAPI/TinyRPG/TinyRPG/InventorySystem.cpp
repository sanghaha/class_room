#include "pch.h"
#include "InventorySystem.h"
#include "Item.h"
#include "ItemData.h"

void InventorySystem::Destroy()
{
	for (int32 i = 0; i < MAX_INVEN_SLOT; ++i)
	{
		SAFE_DELETE(_slots[i]);
		_slots[i] = nullptr;
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
