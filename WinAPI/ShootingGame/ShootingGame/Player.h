#pragma once
#include "Airplane.h"

class Player : public Airplane
{	
	using Super = Airplane;
public:
	Player();
	virtual ~Player();
	
	using Super::Init;
	void Init(Pos pos, wstring bitmapKey);
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

private:
	int32 _hp = 10;
};
