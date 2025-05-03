#pragma once
#include "Actor.h"
#include "AnimSprite.h"
#include "State.h"
#include "ItemData.h"

class Creature : public Actor
{
public:
	using Super = Actor;
public:
	Creature(Vector pos);
	virtual ~Creature();

	void Init() override;
	void Update(float deltaTime) override;
	void Render(ID2D1HwndRenderTarget* renderTarget) override;
	bool IsBlockingCell() override { return true; }

	virtual void SetTexture(Sprite* sprite);
	virtual bool Move(int32 dirX, int32 dirY);

	void ChangeAnimation(AnimType type);
	void ResetAnimation(AnimType type);
	void ChangeState(int32 stateType);


	int32 GetDirX() const { return _dirX; }
	int32 GetDirY() const { return _dirY; }
	AnimInfo* GetCurrAnimation() { return _renderer.GetAimInfo(); }
	int32 GetHp() const { return _hp; }
	int32 GetMaxHp() const { return _maxHp; }
	DirType GetCurrDir() const { return _currDir; }
	int32 GetAttack() const { return _attack; }

	void TakeDamage(int32 damage);
	void ChangeStat(StatType statType, int32 value);

protected:
	virtual void OnDead() {}

private:
	AnimInfo* calcDirAnim(AnimType type);

protected:
	int32			_dirX = 1;
	int32			_dirY = 1;
	DirType			_currDir = DirType::DIR_RIGHT;

	int32			_maxHp = 10;
	int32			_hp = 10;
	int32			_attack = 1;
	int32			_defense = 0;

	AnimSpriteRenderer		_renderer;
	AnimType		_curAnimType = AnimType::A_IDLE;
	AnimInfo		_animInfo[AnimType::A_MAX][DirType::DIR_MAX];

	// 상태 관리
	StateMachine _stateMachine;
};

