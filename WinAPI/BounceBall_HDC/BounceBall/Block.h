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
	void Render(HDC renderTarget) override;
	RenderLayer GetRenderLayer() override;
	MyRect* GetCollisionRect() override;
	ActorType GetActorType() override { return ActorType::AT_BLOCK; }
private:
};

