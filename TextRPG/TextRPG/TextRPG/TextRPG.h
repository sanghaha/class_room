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
    int hp;         // 체력
    int attack;     // 공격력
    int defence;    // 방어력
    int heal;       // 회복력
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


// 외부로 숨길수 있으면 숨기자
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

