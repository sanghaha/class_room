#pragma once
#include "Actor.h"


class Background : public Actor
{
	using Super = Actor;

public:
	Background(Vector pos);
	virtual ~Background();

	void Init() override;
	void Update(float deltaTime) override;
	void Render(ID2D1RenderTarget* renderTarget) override;
	ActorType GetActorType() override;
	RenderLayer GetRenderLayer() override;

private:
};

