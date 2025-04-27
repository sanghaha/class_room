#pragma once
#include "Actor.h"
#include "ColliderCircle.h"
#include "AnimSprite.h"
#include "State.h"

class Creature : public Actor
{
public:
	using Super = Actor;
public:
	Creature(Vector pos);
	virtual ~Creature();

	void Init() override;
	void Update(float deltaTime) override;
	void Render(HDC hdc) override;
	ColliderCircle* GetCollider() override { return &_collider; }

	virtual void SetTexture(Sprite* sprite);
	virtual bool Move(int32 dirX, int32 dirY);

	void ChangeAnimation(AnimType type);
	void ResetAnimation(AnimType type);
	void ChangeState(int32 stateType);


	int32 GetDirX() const { return _dirX; }
	int32 GetDirY() const { return _dirY; }
	float GetMoveSpeed() const { return _moveSpeed; }
	AnimInfo* GetCurrAnimation() { return _renderer.GetAimInfo(); }
	int32 GetHp() const { return _hp; }
	DirType GetCurrDir() const { return _currDir; }
	int32 GetAttack() const { return _attack; }

	void TakeDamage(int32 damage);

private:
	AnimInfo* calcDirAnim(AnimType type);

protected:
	float			_moveSpeed = 300;
	int32			_dirX = 1;
	int32			_dirY = 1;
	DirType			_currDir = DirType::DIR_RIGHT;

	int32			_hp = 10;
	int32			_attack = 1;

	ColliderCircle	_collider;
	AnimSprite		_renderer;
	AnimType		_curAnimType = AnimType::A_IDLE;
	AnimInfo		_animInfo[AnimType::A_MAX][DirType::DIR_MAX];

	// 상태 관리
	StateMachine _stateMachine;
};

