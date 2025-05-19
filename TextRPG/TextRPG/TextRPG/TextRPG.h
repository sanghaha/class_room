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

class Agent
{
public:
    Agent() {}
    Agent(const char* name, int hp, int attack, int defence)
    {
        _name = name;
        _statInfo.hp = hp;
        _statInfo.attack = attack;
        _statInfo.defence = defence;
        _statInfo.heal = 0;
    }
    bool IsDead() { return _statInfo.hp == 0 ? true : false; }
	const char* GetName() { return _name; }
    void TakeDamage(const char* attackerName, int attack);
    virtual void PrintStat();
    void Attack(Agent* target);
    virtual void RoundEnd() {}

protected:
    const char* _name;
    Stat _statInfo;
};

class Character : public Agent
{
public:
    Character() {}
	Character(const char* name, int hp, int attack, int defence) : Agent(name, hp, attack, defence)
	{
        _gold = 0;
	}
    void AddGold(int gold);
    void PrintStat() override;
    void RoundEnd() override;
private:
    int _gold;
};

struct Item
{
    int option = 0;
    int gold = 0;
};

class Monster : public Agent
{
public:
    Monster() {}
    Monster(const char* name, int hp, int attack, int defence) : Agent(name, hp, attack, defence) {}

    void RoundEnd() override;

private:
    Item CreateItemOption();
    void PrintItemOption(Item itemOption);
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



