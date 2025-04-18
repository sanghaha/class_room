#pragma once
#include "Airplane.h"
#include "AnimSprite.h"

class Player : public Airplane
{	
	using Super = Airplane;
public:
	Player(Pos pos);
	virtual ~Player();
	
	void Update(float deltaTime) override;
	void Init() override;
	void Render(HDC hdc) override;
	RenderLayer GetRenderLayer() override { return RenderLayer::RL_Player; }

	void OnEnterCollision(ColliderCircle* src, ColliderCircle* other);
	void OnExitCollision(ColliderCircle* src, ColliderCircle* other);
	void OnOverlapCollision(ColliderCircle* src, ColliderCircle* other);

private:
	void takeDamage();
	void move(float x, float y);

private:
	int32 _hp = 100;

	// hp effect
	AnimSprite _hpTexture;
};

