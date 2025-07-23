#pragma once

#include "Actor.h"


class Block : public Actor
{
	using Super = Actor;

public:
	Block(Vector pos);
	virtual ~Block();

	void Init() override;
	void Update(float deltaTime) override;
	void Render(HDC renderTarget) override;
	RenderLayer GetRenderLayer() override;
	ActorType GetActorType() override { return ActorType::AT_BLOCK; }
private:
};

