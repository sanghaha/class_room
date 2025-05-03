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

    // �κ��丮�� �ִ´�.
    if (false == InventorySystem::GetInstance()->AddItem(_data))
        return;

    // �κ��� ���� �Ǹ� �������� Actor ���� ����.
    Game::GetGameScene()->ReserveRemove(this);
}

InvenItem::~InvenItem()
{
}

void InvenItem::UseItem()
{
    // ������ Ÿ�Կ� ���� �ٸ��� ǥ��
    if (_itemData == nullptr)
        return;

    // �÷��̾� ��������.
    Player* player = Game::GetGameScene()->GetPlayer();
    if (nullptr == player)
        return;

    player->ChangeStat(_itemData->_statType, _itemData->_value);
}

void InvenItem::EquipItem()
{
    // �÷��̾� ��������.
    Player* player = Game::GetGameScene()->GetPlayer();
    if (nullptr == player)
        return;

    player->ChangeStat(_itemData->_statType, _itemData->_value);

    // ���� Ȱ�̶�� ĳ���� ���� ����
    player->ChangeWeapon(_itemData->_weaponType);
}

void InvenItem::UnequipItem()
{
    // �÷��̾� ��������.
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
