#pragma once

#include "Actor.h"


class Block : public SpriteActor
{
	using Super = SpriteActor;

public:
	Block(Vector pos);
	virtual ~Block();

	void Init() override;
	void Update(float deltaTime) override;
	void Render(ID2D1RenderTarget* renderTarget) override;
	RenderLayer GetRenderLayer() override;
	Rect* GetCollisionRect() override;
	ActorType GetActorType() override { return ActorType::AT_BLOCK; }
private:
};

