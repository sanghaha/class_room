#pragma once
#include "Creature.h"

enum PlayerAnimType
{
	PA_IDLE,
	PA_MOVE,
	PA_ATTACK_SIDE,
	PA_ATTACK_DOWN,
	PA_ATTACK_UP,
	PA_MAX
};

class Player : public Creature
{
	using Super = Creature;
public:
	Player(Vector pos);
	virtual ~Player();

	void Init() override;
	void Update(float deltaTime) override;
	void Render(HDC hdc) override;
	RenderLayer GetRenderLayer() override { return RenderLayer::RL_Player; }

	void OnEnterCollision(ColliderCircle* src, ColliderCircle* other);
	void OnExitCollision(ColliderCircle* src, ColliderCircle* other);
	void OnOverlapCollision(ColliderCircle* src, ColliderCircle* other);

	int32 GetHp() const { return _hp; }
	AnimInfo* GetAnimation(PlayerAnimType type) { return &_animInfo[type]; }

	bool Move(int32 dirX, int32 dirY) override;

private:
	void takeDamage();

private:

	AnimInfo	_animInfo[PlayerAnimType::PA_MAX];
};

