#include "TextRPG.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <bitset>
using namespace std;

#define MAX_MONSTER 2

Character playerArr[CharacterType::CT_MAX];
Character* player = nullptr;

Monster monster[MAX_MONSTER];
//Stat playerStat;
//Stat monsterStat;

void InitCharacter()
{
    playerArr[CharacterType::Warrior].name = "전사";
    playerArr[CharacterType::Warrior].stat.hp = 200;
    playerArr[CharacterType::Warrior].stat.attack = 45;
    playerArr[CharacterType::Warrior].stat.defence = 65;

    playerArr[CharacterType::Archer].name = "궁수";
    playerArr[CharacterType::Archer].stat.hp = 100;
    playerArr[CharacterType::Archer].stat.attack = 51;
    playerArr[CharacterType::Archer].stat.defence = 45;

    playerArr[CharacterType::Mage].name = "마법사";
    playerArr[CharacterType::Mage].stat.hp = 100;
    playerArr[CharacterType::Mage].stat.attack = 67;
    playerArr[CharacterType::Mage].stat.defence = 33;
}

void SelectCharacter()
{
    while (true)
    {
        cout << "---------------------------------" << endl;
        cout << "캐릭터를 생성합니다." << endl;
        cout << "---------------------------------" << endl;
        cout << "1. 전사, 2. 궁수, 3. 마법사" << endl;
        cout << ">> ";

        int characterType;
        cin >> characterType;

        player = nullptr;

        if (characterType == Warrior)
        {
            player = &(playerArr[CharacterType::Warrior]);
            return;
        }
        else if (characterType == Archer)
        {
            player = &(playerArr[CharacterType::Archer]);
            return;
        }
        else if (characterType == Mage)
        {
            player = &(playerArr[CharacterType::Mage]);
            return;
        }
    }
}

bool EnterDungeon()
{
    cout << "---------------------------------" << endl;
    cout << "던전에 입장하였습니다." << endl;
    PrintPlayerStat();
    cout << "---------------------------------" << endl;

    // 몬스터 스폰
    CreateRandomMonster();

    cout << "---------------------------------" << endl;
    cout << "1. 도망, 2. 싸운다" << endl;
    cout << ">> ";

    int input;
    cin >> input;

    if (input == 1)
    {
        return false;
    }

    Combat();

    return true;
}

void CreateRandomMonster()
{
    for (int i = 0; i < MAX_MONSTER; ++i)
    {
        int randomChoice = 1 + (rand() % 3);

        switch (randomChoice)
        {
        case MonsterType::Slime:
            monster[i].name = "슬라임";
            monster[i].stat.hp = 10;
            monster[i].stat.attack = 10;
            monster[i].stat.defence = 0;
            break;
        case MonsterType::Orc:
            monster[i].name = "오크";
            monster[i].stat.hp = 20;
            monster[i].stat.attack = 10;
            monster[i].stat.defence = 0;
            break;
        case MonsterType::Dragon:
            monster[i].name = "드래곤";
            monster[i].stat.hp = 30;
            monster[i].stat.attack = 10;
            monster[i].stat.defence = 0;
            break;
        }

        PrintStat(monster[i].name, monster[i].stat, nullptr);
    }
}

void Combat()
{
    cout << "---------------------------------" << endl;
    cout << "전투를 시작합니다. 누군가 죽을때까지 전투는 계속됩니다!" << endl;
    cout << "---------------------------------" << endl;
    for (int i = 0; i < MAX_MONSTER; ++i)
    {
        cout << "<<<< " << monster[i].name << ">>>>" << endl;
        while (true)
        {
            int random = rand() % 100;

            // 50% 확률로 공격 성공    
            if (random < 50)
            {
                cout << player->name << " 공격 실패.." << endl;
            }
            else
            {
                // 플레이어 공격
                bool dead = Attack(player->name, monster[i].name, (monster[i].stat.hp), player->stat.attack, monster[i].stat.defence);
                if (dead)
                {
                    cout << monster[i].name << "를 처치했습니다!" << endl;

                    // 아이템 속성 생성
                    Item item = CreateItemOption();
                    player->gold += item.gold;
                    PrintItemOption(item);

                    cout << player->name << "의 소지금이 늘어납니다! " << player->gold << endl;
                    cout << "#################################" << endl;
                    break;
                }
            }

            random = rand() % 100;
            // 50% 확률로 몬스터 공격 성공        
            if (random < 50)
            {
                // 몬스터 공격
                bool dead = Attack(monster[i].name, player->name, (player->stat.hp), monster[i].stat.attack, player->stat.defence);
                if (dead)
                {
                    cout << player->name << " 죽었습니다...." << endl;
                    break;
                }
            }
            else
            {
                cout << monster[i].name << " 공격 실패.." << endl;
            }

            cout << "---------------------------------" << endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}

bool CheckNextGame()
{
    cout << "---------------------------------" << endl;
    cout << "1. 재도전, 2. 종료" << endl;
    cout << "---------------------------------" << endl;

    int input;
    cin >> input;

    if (input == 2)
    {
        return false;
    }

    return true;
}

void PrintStat(const char* name, Stat& stat, int* gold)
{
    cout << " [ " << name;
    if (gold != nullptr)
        cout << " , 소지금 : " << *gold;
    cout << " , HP : " << stat.hp;
    cout << " , ATK : " << stat.attack;
    cout << " , DEF : " << stat.defence;
    cout << " , Heal : " << stat.heal << " ]" << endl;
}

void PrintPlayerStat()
{
    if (player == nullptr)
        return;

    PrintStat(player->name, player->stat, &player->gold);
}

Item CreateItemOption()
{
    Item item;
    item.gold = rand() % 100;

    for (int i = 0; i < ItemOptionMax; ++i)
    {
        // 50% 확률로 옵션 부여
        int rate = rand() % 100;
        if (rate < 50)
        {
            int option = (1 << i);
            item.option |= option;
            //cout << "옵션 부여 : " << bitset<4>(option) << endl;
        }
    }

    return item;
}

void PrintItemOption(Item item)
{
    cout << "가격 : " << item.gold << endl;

    for (int option = 0; option < ItemOptionMax; ++option)
    {
        int checkBit = (1 << option);

        if (item.option & checkBit)
        {
            switch (option)
            {
            case ItemOption::Sell:
            {
                cout << "팔수 있음" << endl;
            }break;
            case ItemOption::Drop:
            {
                cout << "드랍 가능" << endl;
            }break;
            case ItemOption::Enchant:
            {
                cout << "강화 가능" << endl;
            }break;
            }
        }
    }
}

// 반환값 : 죽였는지 여부
bool Attack(const char* attackerName, const char* targetName, int& hp, int attack, int defence)
{
    int damage = attack - defence;
    hp -= damage;
    hp = (hp < 0) ? 0 : hp; // HP가 음수가 되지 않도록 처리

    cout << "[" << attackerName << " 공격] 피해량: " << damage << endl;
    cout << "[" << targetName << " ] HP : " << hp << endl;

    return hp == 0 ? true : false;
}
