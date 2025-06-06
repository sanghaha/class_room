﻿#include "TextRPG.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <bitset>
using namespace std;

#define MAX_MONSTER 2

Character playerArr[CharacterType::CT_MAX];
Character* player = nullptr;

Monster monsterArr[MAX_MONSTER];
Monster* monster = nullptr;
//Stat playerStat;
//Stat monsterStat;

void InitCharacter()
{
    playerArr[CharacterType::Warrior] = Character("전사", 200, 45, 65);
    playerArr[CharacterType::Archer] = Character("궁수", 100, 51, 45);
    playerArr[CharacterType::Mage] = Character("마법사", 100, 67, 33);
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
            break;
        }
        else if (characterType == Archer)
        {
            player = &(playerArr[CharacterType::Archer]);
            break;
        }
        else if (characterType == Mage)
        {
            player = &(playerArr[CharacterType::Mage]);
            break;
        }
    }

    player->PrintStat();
}

bool EnterDungeon()
{
    if (player == nullptr)
        return false;

    cout << "---------------------------------" << endl;
    cout << "던전에 입장하였습니다." << endl;
    player->PrintStat();
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
            monsterArr[i] = Monster("슬라임", 10, 10, 0);
            break;
        case MonsterType::Orc:
            monsterArr[i] = Monster("오크", 20, 10, 0);
            break;
        case MonsterType::Dragon:
            monsterArr[i] = Monster("드래곤", 30, 10, 0);
            break;
        }

        monsterArr[i].PrintStat();
    }
}

void Combat()
{
    cout << "---------------------------------" << endl;
    cout << "전투를 시작합니다. 누군가 죽을때까지 전투는 계속됩니다!" << endl;
    cout << "---------------------------------" << endl;
    for (int i = 0; i < MAX_MONSTER; ++i)
    {
        // 현재 싸우고 있는 몬스터
        monster = &monsterArr[i];

        cout << "<<<< " << monster->GetName() << ">>>>" << endl;
        while (true)
        {
            player->Attack(monster);
            if (monster->IsDead())
            {
				cout << monster->GetName() << "를 처치했습니다!" << endl;

                break;
            }

			monster->Attack(player);
            if (player->IsDead())
            {
                cout << player->GetName() << " 죽었습니다...." << endl;
                return;
            }

            cout << "---------------------------------" << endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        // 여기가 전투가 끝난 시점이다
        monster->RoundEnd();
        player->RoundEnd();

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

//-------------------------------------------------
// Agent Class
//-------------------------------------------------
void Agent::TakeDamage(const char* attackerName, int attack)
{
    int damage = attack - _statInfo.defence;
    _statInfo.hp -= damage;
    _statInfo.hp = (_statInfo.hp < 0) ? 0 : _statInfo.hp; // HP가 음수가 되지 않도록 처리

    cout << "[" << attackerName << " 공격] 피해량: " << damage << endl;
    cout << "[" << _name << " ] HP : " << _statInfo.hp << endl;
}

void Agent::PrintStat()
{
    cout << " [ " << _name;
    cout << " , HP : " << _statInfo.hp;
    cout << " , ATK : " << _statInfo.attack;
    cout << " , DEF : " << _statInfo.defence;
    cout << " , Heal : " << _statInfo.heal << " ]" << endl;
}

void Agent::Attack(Agent* target)
{
    if (nullptr == target)
        return;    // 예외처리

    bool dead = false;
    int random = rand() % 100;

    // 50% 확률로 공격 성공    
    if (random < 50)
    {
        cout << _name << " 공격 실패.." << endl;
    }
    else
    {
        target->TakeDamage(_name, _statInfo.attack);
    }
}

//-------------------------------------------------
// Character Class
//-------------------------------------------------
// 현재 캐릭터의 스탯 출력
void Character::PrintStat()
{
    cout << "[ 소지금 : " << _gold << " ]";
    Agent::PrintStat();

}

void Character::RoundEnd()
{
	_statInfo.hp += _statInfo.heal;
    cout << _name << "가 회복합니다. " << _statInfo.heal << endl;
}


void Character::AddGold(int gold)
{
    _gold += gold;

	if (_gold < 0)
	{
		_gold = 0;
	}

    if (gold < 0)
    {
        cout << _name << "의 소지금이 사라졌습니다..??? " << gold << endl;
    }
    else
    {
        cout << _name << "의 소지금이 늘어납니다! " << _gold << endl;
    }
    cout << "#################################" << endl;
}



//-------------------------------------------------
// Monster Class
//-------------------------------------------------

Item Monster::CreateItemOption()
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

void Monster::PrintItemOption(Item item)
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

void Monster::RoundEnd()
{
    // 아이템 속성 생성
    Item item = CreateItemOption();
    PrintItemOption(item);
    player->AddGold(item.gold);

	int random = rand() % 50;
    player->AddGold(-random);
}
