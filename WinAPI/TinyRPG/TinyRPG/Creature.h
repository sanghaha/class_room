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

	void ChangeAnimation(AnimInfo* animInfo);
	void ChangeState(int32 stateType);

	int32 GetDirX() const { return _dirX; }
	int32 GetDirY() const { return _dirY; }
	float GetMoveSpeed() const { return _moveSpeed; }

protected:
	float _moveSpeed = 300;
	int32 _dirX = 1;
	int32 _dirY = 1;
	int32 _hp = 10;

	ColliderCircle _collider;
	AnimSprite _renderer;

	// 상태 관리
	StateMachine _stateMachine;
};

