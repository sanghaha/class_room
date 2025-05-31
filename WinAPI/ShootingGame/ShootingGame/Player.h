#pragma once
#include "Airplane.h"

class Player : public Airplane
{	
	using Super = Airplane;
public:
	Player(Pos pos, wstring bitmapKey);
	virtual ~Player();
	
	void Update(float deltaTime) override;
	void Init() override;
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

