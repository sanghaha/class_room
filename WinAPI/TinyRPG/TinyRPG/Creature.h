#pragma once
#include "Actor.h"
#include "AnimSprite.h"
#include "State.h"
#include "ItemData.h"

class Creature : public AnimSpriteActor
{
public:
	using Super = AnimSpriteActor;
public:
	Creature(Vector pos);
	virtual ~Creature();

	void Init() override;
	void Update(float deltaTime) override;
	void Render(ID2D1RenderTarget* renderTarget) override;
	bool IsBlockingCell() override { return true; }

	virtual bool Move(Cell destCell);

	void ChangeAnimation(AnimType type);
	void ChangeState(int32 stateType);


	int32 GetDirX() const { return _dirX; }
	int32 GetDirY() const { return _dirY; }
	int32 GetHp() const { return _hp; }
	int32 GetMaxHp() const { return _maxHp; }
	DirType GetCurrDir() const { return _currDir; }
	void SetCurrDir(DirType dir) { _currDir = dir; }
	int32 GetAttack() const { return _attack; }

	virtual bool CanAttackToTarget(Creature* target);
	virtual void Attack();
	void TakeDamage(int32 damage);
	void ChangeStat(StatType statType, int32 value);

protected:
	virtual void OnDead() {}
	virtual AnimInfo* calcDirAnim(AnimType type);

protected:
	int32			_dirX = 1;
	int32			_dirY = 1;
	DirType			_currDir = DirType::DIR_RIGHT;
	bool			_lastRenderFlipX = false;

	int32			_maxHp = 10;
	int32			_hp = 10;
	int32			_attack = 1;
	int32			_defense = 0;

	AnimType		_curAnimType = AnimType::A_IDLE;
	//AnimInfo*		_spriteInfo[AnimType::A_MAX];

	// 상태 관리
	StateMachine _stateMachine;
};

