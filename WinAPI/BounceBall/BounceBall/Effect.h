#pragma once
#include "Actor.h"

class Effect : public AnimSpriteActor
{
	using Super = AnimSpriteActor;

public:
	Effect(Vector pos);
	virtual ~Effect();


	RenderLayer GetRenderLayer() override { return RenderLayer::RL_Effect; }
	ActorType GetActorType() override { return ActorType::AT_EFFECT; }

private:
};

