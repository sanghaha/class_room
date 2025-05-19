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

class Character
{
public:
    Character() {}
	Character(const char* name, int hp, int attack, int defence)
	{
        _name = name;
		_statInfo.hp = hp;
		_statInfo.attack = attack;
		_statInfo.defence = defence;
        _statInfo.heal = 0;
        _gold = 0;
	}
    void PrintPlayerStat();
    bool Attack(const char* targetName, int& hp, int defence);

    const char* _name;
    Stat _statInfo;
    int _gold;
};

struct Item
{
    int option = 0;
    int gold = 0;
};

class Monster
{
public:
    Monster() {}
    Monster(const char* name, int hp, int attack, int defence)
    {
		_name = name;
		_statInfo.hp = hp;
		_statInfo.attack = attack;
		_statInfo.defence = defence;
        _statInfo.heal = 0;
    }
    bool Attack(const char* targetName, int& hp, int defence);

    const char* _name;
    Stat _statInfo;
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

Item CreateItemOption();
void PrintItemOption(Item itemOption);
bool ApplyDamage(const char* attackerName, const char* targetName, int& hp, int attack, int defence);

