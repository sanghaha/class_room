#pragma once
#include "Creature.h"
#include "AnimSprite.h"

enum PlayerState
{
	P_IDLE,
	P_WALK,
	P_SIDE_ATTACK,
	P_DOWN_ATTACK,
	P_UP_ATTACk,
	P_MAX,
};

class Player : public Creature
{
	using Super = Creature;
public:
	Player(Pos pos);
	virtual ~Player();

	void Init() override;
	void Update(float deltaTime) override;
	void Render(HDC hdc) override;
	RenderLayer GetRenderLayer() override { return RenderLayer::RL_Player; }

	void OnEnterCollision(ColliderCircle* src, ColliderCircle* other);
	void OnExitCollision(ColliderCircle* src, ColliderCircle* other);
	void OnOverlapCollision(ColliderCircle* src, ColliderCircle* other);

	int32 GetHp() const { return _hp; }

private:
	void takeDamage();
	void move(float x, float y);
	void changeState(PlayerState state);

private:

	int32 _hp = 10;
	int8 _curState = -1;
	AnimInfo _animInfo[PlayerState::P_MAX];
};

