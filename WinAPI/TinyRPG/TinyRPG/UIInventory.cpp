#include "pch.h"
#include "UIInventory.h"
#include "Texture.h"
#include "Sprite.h"
#include "ResourceManager.h"
#include "InventorySystem.h"
#include "ItemData.h"
#include "InputManager.h"
#include "Item.h"

UIInventory::UIInventory()
{
}

UIInventory::~UIInventory()
{
}

void UIInventory::Init()
{
	_equipBG = dynamic_cast<PNGTexture*>(ResourceManager::GetInstance()->GetTexture(L"EquipBG"));
	_invenBG = dynamic_cast<PNGTexture*>(ResourceManager::GetInstance()->GetTexture(L"InventoryBG"));
	_selectISlotBG = dynamic_cast<PNGTexture*>(ResourceManager::GetInstance()->GetTexture(L"InventorySelected"));

	Sprite* sprite = ResourceManager::GetInstance()->GetSprite(L"Items");
	SpriteRenderInfo info;
	info.width = 36;
	info.height = 36;
	info.alignCenter = false;
	info.applyCamera = false;
	_invenIconRenderer.SetSprite(sprite, info);

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
}

void UIInventory::Update()
{
	// 아이템 선택했는지 여부
	if (InputManager::GetInstance()->GetButtonDown(KeyType::LeftMouse))
	{
		// 장착 슬롯 체크
		if (checkClickSlot(0, MAX_EQUIP_SLOT, [](int32 slotIdx)
			{
				InventorySystem::GetInstance()->UnequipItem((ItemSlot)slotIdx);	
			}))
		{
			return;
		}

		// 인벤 슬롯 체크
		if (checkClickSlot(MAX_EQUIP_SLOT, MAX_INVEN_SLOT, [](int32 slotIdx)
			{
				int32 invenSlotIdx = slotIdx - MAX_EQUIP_SLOT;
				InventorySystem::GetInstance()->UseItem(invenSlotIdx);
			}))
		{
			return;
		}
	}
}

void UIInventory::Render(ID2D1HwndRenderTarget* renderTarget)
{
	if (_isOpen == false)
		return;

	// equip
	{
		_equipBG->Render(renderTarget, _bgEquipPos);
	}

	// inventory
	{
		_invenBG->Render(renderTarget, _bgInvenPos);
	}

	// selected
	if (_selectedIndex != -1)
	{
		_selectISlotBG->Render(renderTarget, _slotPos[_selectedIndex] - Vector(5, 5));
	}

	// 아이템 표현
	for (int32 i = 0; i < MAX_EQUIP_SLOT + MAX_INVEN_SLOT; ++i)
	{
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

		const SpriteIndex* spriteInfo = ResourceManager::GetInstance()->GetItemSpriteIndex(item->GetSpriteName());
		if (nullptr == spriteInfo)
			continue;

		_invenIconRenderer.SetIndex(spriteInfo->indexX, spriteInfo->indexY);
		_invenIconRenderer.Render(renderTarget, _slotPos[i]);
	}
}

void UIInventory::Open()
{
	Super::Open();

	_selectedIndex = -1;
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
