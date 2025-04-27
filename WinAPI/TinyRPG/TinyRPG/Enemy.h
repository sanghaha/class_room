#pragma once
#include "Creature.h"

class Enemy : public Creature
{
	using Super = Creature;
public:
	Enemy(Vector pos);
	virtual ~Enemy();

	void Init() override;
	void Update(float deltaTime) override;
	void Render(HDC hdc) override;
	RenderLayer GetRenderLayer() override { return RenderLayer::RL_Enemy; }

	void OnEnterCollision(ColliderCircle* src, ColliderCircle* other);
	void OnExitCollision(ColliderCircle* src, ColliderCircle* other);
	void OnOverlapCollision(ColliderCircle* src, ColliderCircle* other);


	bool Move(int32 dirX, int32 dirY) override;
};

