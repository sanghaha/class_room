#pragma once

#include "Actor.h"
#include "Sprite.h"
#include "ItemData.h"

class Sprite;

class DropItem : public SpriteActor
{
	using Super = SpriteActor;
public:
	DropItem(Vector pos, const ItemData* itemData);
	virtual ~DropItem();

	virtual void Init() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(ID2D1RenderTarget* renderTarget) override;
	bool IsBlockingCell() override { return false; }

	virtual RenderLayer GetRenderLayer() override;

	void PickupItem();

private:
	const ItemData* _data = nullptr;
};

class InvenItem
{
public:
	InvenItem(const ItemData* itemData) : _itemData(itemData) {}
	virtual ~InvenItem();

	void UseItem();
	void EquipItem();
	void UnequipItem();

	string GetSpriteName();
	ItemSlot GetItemSlotType();
private:
	const ItemData* _itemData;
};
