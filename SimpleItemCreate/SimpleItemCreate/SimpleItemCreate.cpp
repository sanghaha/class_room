// SimpleItemCreate.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <string>
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new new (_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

void EnableMemoryLeakDetection() {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
}

using namespace std;

enum ItemType
{
    Weapon,
    Armor,
    Acc,
    Shoes,
    ItemType_MAX,
};

enum ItemOption
{
    Sell,
    Drop,
    Enchant,
    ItemOption_MAX,
};

struct Item
{
    ItemType type;
    int price;
    int option;
};

std::string g_ItemName[ItemType_MAX] = {"무기", "방어구", "악세사리", "신발"};

Item** g_EquipList = nullptr; // Item* 를 저장하는 동적 배열
int g_EquipItemCount = 0;


Item* CreateItem();
void ShowItemInfo(Item* item);
int FindItem(ItemType type);
void EquipItem(Item* item);
void UnEquipItem(Item* item);
void UnEquipItemAll();
void PrintAllItem();

int main()
{
    EnableMemoryLeakDetection();

    srand(time(0));

    Item* newItem = nullptr;

    bool run = true;
    while (run)
    {
        cout << "*********** 0:종료, 1:생성, 2:장착, 3:해제, 4:모두 착용 해제, 5:출력 ********* " << endl;
        cout << ">>";
        int input;
        cin >> input;

        switch (input)
        {
        case 0: 
        {
            run = false;
            break;
        }
        case 1: 
        {
            if (newItem)
            {
                delete newItem;
                newItem = nullptr;
            }

            newItem = CreateItem();
            ShowItemInfo(newItem);
        }break;
        case 2:
        {
            EquipItem(newItem);
            PrintAllItem();

            newItem = nullptr;
        }break;
        case 3:
        {
            cout << "해제할 슬롯을 선택하세요. 1.무기, 2.방어구, 3.악세사리, 4.신발" << endl;
            cout << ">>";

            int slot;
            cin >> slot;
            int findSlotIndex = FindItem((ItemType)(slot - 1));
            if (findSlotIndex == -1)
            {
                cout << "미장착 슬롯입니다." << endl;
            }
            else
            {
                UnEquipItem(g_EquipList[findSlotIndex]);
                PrintAllItem();
            }
        }break;
        case 4:
        {
            UnEquipItemAll();
            PrintAllItem();
        }break;
        case 5:
        {
            PrintAllItem();
        }break;

        }

        cout << endl;
    }

    if (newItem)
    {
        delete newItem;
        newItem = nullptr;
    }
    UnEquipItemAll();
}



Item* CreateItem()
{
    Item* item = new Item();
    item->type = (ItemType)(rand() % ItemType_MAX);
    item->price = rand() % 100;

    for (int i = 0; i < ItemOption_MAX; ++i)
    {
        if (rand() % 2 == 0)
        {
            int bit = (1 << i);
            item->option |= bit;
        }
    }

    return item;
}

void ShowItemInfo(Item* item)
{
    if (item == nullptr)
        return;

    cout << "[ " << g_ItemName[item->type].c_str() << " ], 가격:" << item->price;
    cout << ", 옵션:";
    for (int i = 0; i < ItemOption_MAX; ++i)
    {
        int bit = 1 << i;
        if (item->option & bit)
        {
            switch ((ItemOption)i)
            {
            case ItemOption::Sell: cout << "(판매 가능)";  break;
            case ItemOption::Drop: cout << "(드랍 가능)";  break;
            case ItemOption::Enchant: cout << "(강화 가능)";  break;
            }
        }
    }
    cout << endl;
}

int FindItem(ItemType type)
{
    for (int i = 0; i < g_EquipItemCount; ++i)
    {
        if (g_EquipList[i] && g_EquipList[i]->type == type)
        {
            return i;
        }
    }

    return -1;
}

void EquipItem(Item* item)
{
    if (item == nullptr)
        return;

    int findSlotIndex = FindItem(item->type);

    // 기존에 장착중인 슬롯이 있다.
    if (findSlotIndex != -1)
    {
        delete g_EquipList[findSlotIndex];

        // 기존 슬롯을 교체
        g_EquipList[findSlotIndex] = item;
    }
    // 새로운 슬롯을 추가해야 한다.
    else
    {
        Item** newItemList = new Item * [g_EquipItemCount + 1];
        for (int i = 0; i < g_EquipItemCount; ++i)
        {
            newItemList[i] = g_EquipList[i];
        }
        newItemList[g_EquipItemCount] = item;

        // 기존 아이템 배열은 해제
        delete[] g_EquipList; // Item* 배열 자체 해제

        // 새로운 배열 선언
        g_EquipList = newItemList;
        g_EquipItemCount++;
    }
}

void UnEquipItem(Item* item)
{
    if (item == nullptr)
        return;

    if (g_EquipItemCount == 1)
    {
        UnEquipItemAll();
    }
    else
    {
        Item** newItemList = new Item * [g_EquipItemCount - 1];
        int newItemIndex = 0;
        for (int i = 0; i < g_EquipItemCount; ++i)
        {
            if (g_EquipList[i] != item)
            {
                newItemList[newItemIndex] = g_EquipList[i];
                newItemIndex++;
            }
        }

        // 아이템 삭제
        delete item;

        // 기존 아이템 배열은 해제
        delete[] g_EquipList; // Item* 배열 자체 해제

        // 새로운 배열 선언
        g_EquipList = newItemList;
        g_EquipItemCount--;
    }
}

void UnEquipItemAll()
{
    for (int i = 0; i < g_EquipItemCount; ++i)
    {
        if (g_EquipList[i])
        {
            delete g_EquipList[i]; // 각각의 Item 객체 해제
        }
    }

    delete[] g_EquipList; // Item* 배열 자체 해제
    g_EquipList = nullptr;
    g_EquipItemCount = 0;
}

void PrintAllItem()
{
    cout << "<<현재 장착중인 아이템>>" << endl;
    cout << "---------------------" << endl;
    for (int i = 0; i < g_EquipItemCount; ++i)
    {
        if (g_EquipList[i])
        {
            ShowItemInfo(g_EquipList[i]);
        }
    }
    cout << "---------------------" << endl;
}

