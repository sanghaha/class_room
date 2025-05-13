#include "pch.h"
#include "UIInventory.h"
#include "Texture.h"
#include "Sprite.h"
#include "UIImage.h"
#include "ResourceManager.h"
#include "InventorySystem.h"
#include "ItemData.h"
#include "InputManager.h"
#include "Item.h"

UIInventory::UIInventory()
{
	_isOpen = false;

	// 위치값 저장
	{
		_bgEquipPos = Vector(GWinSizeX - 220, GWinSizeY * 0.1f);
		_bgInvenPos = Vector(_bgEquipPos.x + 46, _bgEquipPos.y + 120);

		{
			Vector itemStartPos = _bgEquipPos;
			itemStartPos.x += 15;
			itemStartPos.y += 15;

			for (int32 i = 0; i < MAX_EQUIP_SLOT; ++i)
			{
				_slotPos[i].x = itemStartPos.x + ((i % 4) * (40 + 6));
				_slotPos[i].y = itemStartPos.y + ((i / 4) * (40 + 6));
			}
		}
		{
			Vector itemStartPos = _bgInvenPos;
			itemStartPos.x += 15;
			itemStartPos.y += 15;

			for (int32 i = 0; i < MAX_INVEN_SLOT; ++i)
			{
				_slotPos[i + MAX_EQUIP_SLOT].x = itemStartPos.x + ((i % 3) * (40 + 6));
				_slotPos[i + MAX_EQUIP_SLOT].y = itemStartPos.y + ((i / 3) * (40 + 6));
			}
		}
	}

	{
		CreateImage(_bgEquipPos, "EquipBG");
		CreateImage(_bgInvenPos, "InventoryBG");
		_selectISlotBG = CreateImage(Vector(0,0), "InventorySelected", 48, 48);

		for (int32 i = 0; i < MAX_EQUIP_SLOT + MAX_INVEN_SLOT; ++i)
		{
			_itemIconRenderer[i] = CreateSprite(_slotPos[i], "UI_Meat1", 36, 36);
		}
	}
}

UIInventory::~UIInventory()
{
}

void UIInventory::Init()
{
	//SpriteRenderInfo info;
	//info.width = 36;
	//info.height = 36;
	//info.alignCenter = false;
	//info.applyCamera = false;

	/*
	_equipBG = new PNGTexture(L"EquipBG");
	_invenBG = new PNGTexture(L"InventoryBG");
	_selectISlotBG = new PNGTexture(L"InventorySelected", 48, 48);

	_invenIconRenderer = new Sprite(L"Items");
	_invenIconRenderer->SetInfo(info);


	*/
}

void UIInventory::Update(float deltaTime)
{
	if (_isOpen == false)
		return;

	Super::Update(deltaTime);

	if (_selectISlotBG)
	{
		_selectISlotBG->SetVisible(_selectedIndex != -1);

		if (_selectedIndex != -1)
		{
			_selectISlotBG->SetPos(_slotPos[_selectedIndex] - Vector(5, 5));
		}
	}

	// 아이템 표현
	for (int32 i = 0; i < MAX_EQUIP_SLOT + MAX_INVEN_SLOT; ++i)
	{
		if (nullptr == _itemIconRenderer[i])
			continue;

		_itemIconRenderer[i]->SetVisible(false);

		InvenItem* item = nullptr;

		if (i < MAX_EQUIP_SLOT)
		{
			item = InventorySystem::GetInstance()->GetEqiupItem((ItemSlot)i);
		}
		else
		{
			item = InventorySystem::GetInstance()->GetItem(i - MAX_EQUIP_SLOT);
		}

		if (nullptr == item)
			continue;

		const SpriteInfo* spriteInfo = ResourceManager::GetInstance()->GetSpriteInfo(item->GetSpriteName());
		if (nullptr == spriteInfo)
			continue;

		_itemIconRenderer[i]->SetVisible(true);
		_itemIconRenderer[i]->SetIndex(spriteInfo->indexX, spriteInfo->indexY);
	}
}

void UIInventory::Render(ID2D1RenderTarget* renderTarget)
{
	if (_isOpen == false)
		return;

	Super::Render(renderTarget);
}

void UIInventory::Open()
{
	Super::Open();

	_selectedIndex = -1;
}

bool UIInventory::OnLeftClickEvent(int32 x, int32 y)
{
	if (!_isOpen)
		return false;

	// 아이템 선택했는지 여부
		// 장착 슬롯 체크
	if (checkClickSlot(0, MAX_EQUIP_SLOT, [](int32 slotIdx)
		{
			InventorySystem::GetInstance()->UnequipItem((ItemSlot)slotIdx);
		}))
	{
		return true;
	}

	// 인벤 슬롯 체크
	if (checkClickSlot(MAX_EQUIP_SLOT, MAX_INVEN_SLOT, [](int32 slotIdx)
		{
			int32 invenSlotIdx = slotIdx - MAX_EQUIP_SLOT;
			InventorySystem::GetInstance()->UseItem(invenSlotIdx);
		}))
	{
		return true;
	}
	return false;
}

bool UIInventory::checkClickSlot(int32 start, int32 maxCount, std::function<void(int32)> onClickSameSlot)
{
	POINT mousePos = InputManager::GetInstance()->GetMousePos();

	RECT slotRect;
	for (int32 i = start; i < start + maxCount; ++i)
	{
		slotRect.left = (int32)_slotPos[i].x;
		slotRect.top = (int32)_slotPos[i].y;
		slotRect.right = slotRect.left + _slotSize;
		slotRect.bottom = slotRect.top + _slotSize;

		if (IsInPoint(slotRect, mousePos))
		{
			if (_selectedIndex == i)
			{
				// 동일한 슬롯 한번더 선택. 아이템 사용
				onClickSameSlot(i);
				_selectedIndex = -1;
				return true;
			}
			else
			{
				_selectedIndex = i;
				return true;
			}
		}
	}

	return false;
}
