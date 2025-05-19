#pragma once


enum CharacterType
{
    Warrior = 1,
    Archer,
    Mage,
    CT_MAX,
};

enum MonsterType
{
    Slime = 1,
    Orc,
    Dragon
};

enum ItemOption
{
    Sell,
    Drop,
    Enchant,
    ItemOptionMax
};

enum Menu
{
    Menu_EnterDungeon = 1,
    Menu_SelectCharacter,
    Menu_Quit
};

struct Stat
{
    int hp;         // ü��
    int attack;     // ���ݷ�
    int defence;    // ����
    int heal;       // ȸ����
};

struct Character
{
    const char* name;
    Stat stat;
    int gold = 0;
};

struct Item
{
    int option = 0;
    int gold = 0;
};

struct Monster
{
    const char* name;
    Stat stat;
};


// �ܺη� ����� ������ ������
//extern Stat playerStat;
//extern Stat monsterStat;
void InitCharacter();
void SelectCharacter();
bool EnterDungeon();
void CreateRandomMonster();
void Combat();
bool CheckNextGame();
void PrintStat(const char* name, Stat& stat, int* gold);
void PrintPlayerStat();
Item CreateItemOption();
void PrintItemOption(Item itemOption);
bool Attack(const char* attackerName, const char* targetName, int& hp, int attack, int defence);

