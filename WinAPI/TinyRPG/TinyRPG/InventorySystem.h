#pragma once
#include "Singleton.h"
#include "ItemData.h"

class InvenItem;
class InventorySystem : public Singleton<InventorySystem>
{
public:
	void Destroy() override;

	bool AddItem(const ItemData* data);
	void UseItem(int32 slotIdx);
	InvenItem* GetItem(int32 slotIdx);

	bool EquipItem(int32 invenSlotIdx);
	bool UnequipItem(ItemSlot slot);
	InvenItem* GetEqiupItem(ItemSlot slot);

	bool IsFull() const;

private:
	int32 getEmptySlotIdx() const;

private:
	InvenItem* _equips[MAX_EQUIP_SLOT];
	InvenItem* _slots[MAX_INVEN_SLOT];
};

