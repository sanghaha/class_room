#pragma once
#include "Actor.h"

class Star : public SpriteActor
{
	using Super = SpriteActor;

public:
	Star(Vector pos);
	virtual ~Star();

	void Init() override;
	void Update(float deltaTime) override;
	void Render(ID2D1RenderTarget* renderTarget) override;
	RenderLayer GetRenderLayer() override;
	Rect* GetCollisionRect() override;
	ActorType GetActorType() override { return ActorType::AT_STAR; }
private:
};