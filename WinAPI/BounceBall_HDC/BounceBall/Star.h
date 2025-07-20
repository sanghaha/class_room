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
	void Render(HDC renderTarget) override;
	RenderLayer GetRenderLayer() override;
	MyRect* GetCollisionRect() override;
	ActorType GetActorType() override { return ActorType::AT_STAR; }
private:
};