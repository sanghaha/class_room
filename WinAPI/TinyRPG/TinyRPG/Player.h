#pragma once
#include "Creature.h"

class Player : public Creature
{
	using Super = Creature;
public:
	Player(Vector pos);
	virtual ~Player();

	void Init() override;
	void Update(float deltaTime) override;
	void Render(ID2D1HwndRenderTarget* renderTarget) override;
	RenderLayer GetRenderLayer() override { return RenderLayer::RL_Player; }
	void OnBeginOverlapActor(Actor* other) override;
	void OnEndOverlapActor(Actor* other) override;

	bool Move(int32 dirX, int32 dirY) override;

protected:
	void OnDead() override;

private:


};

