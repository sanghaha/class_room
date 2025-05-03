#include "pch.h"
#include "Item.h"
#include "ItemData.h"
#include "ResourceManager.h"
#include "InventorySystem.h"
#include "Game.h"
#include "GameScene.h"
#include "Player.h"

DropItem::DropItem(Vector pos, const ItemData* itemData) : Super(pos), _data(itemData)
{
    
}

DropItem::~DropItem()
{
}

void DropItem::Init()
{
    Super::Init();

    if (_data)
    {
        const SpriteIndex* index = ResourceManager::GetInstance()->GetItemSpriteIndex(_data->_spriteName);
        if (index)
        {
            SpriteRenderInfo info;
            info.indexX = index->indexX;
            info.indexY = index->indexY;
            info.width = GTileSize - 10;
            info.height = GTileSize - 10;

            Sprite* sprite = ResourceManager::GetInstance()->GetSprite(L"Items");
            _renderer.SetSprite(sprite, info);
        }
    }
}

void DropItem::Update(float deltaTime)
{
   
}

void DropItem::Render(ID2D1HwndRenderTarget* renderTarget)
{
    _renderer.Render(renderTarget, GetPos());
}

RenderLayer DropItem::GetRenderLayer()
{
    return RenderLayer::RL_Item;
}

void DropItem::PickupItem()
{
    if (InventorySystem::GetInstance()->IsFull())
        return;

    // 인벤토리에 넣는다.
    if (false == InventorySystem::GetInstance()->AddItem(_data))
        return;

    // 인벤에 들어가게 되면 아이템은 Actor 개념 삭제.
    Game::GetGameScene()->ReserveRemove(this);
}

InvenItem::~InvenItem()
{
}

void InvenItem::UseItem()
{
    // 아이템 타입에 따라 다르게 표현
    if (_itemData == nullptr)
        return;

    // 플레이어 가져오기.
    Player* player = Game::GetGameScene()->GetPlayer();
    if (nullptr == player)
        return;

    player->ChangeStat(_itemData->_statType, _itemData->_value);
}

void InvenItem::EquipItem()
{
    // 플레이어 가져오기.
    Player* player = Game::GetGameScene()->GetPlayer();
    if (nullptr == player)
        return;

    player->ChangeStat(_itemData->_statType, _itemData->_value);

    // 만약 활이라면 캐릭터 외형 변경
    player->ChangeWeapon(_itemData->_weaponType);
}

void InvenItem::UnequipItem()
{
    // 플레이어 가져오기.
    Player* player = Game::GetGameScene()->GetPlayer();
    if (nullptr == player)
        return;

    player->ChangeStat(_itemData->_statType, -_itemData->_value);
}

string InvenItem::GetSpriteName()
{
    if (_itemData) return _itemData->_spriteName;
    return "";
}

ItemSlot InvenItem::GetItemSlotType()
{
    if (_itemData) return _itemData->_equipSlot;
    return ItemSlot::NoneSlot;
}
