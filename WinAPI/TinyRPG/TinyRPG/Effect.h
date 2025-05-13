#pragma once
#include "Actor.h"
#include "AnimSprite.h"

class Effect : public AnimSpriteActor
{
	using Super = AnimSpriteActor;
public:
	Effect(Vector pos, string spriteName);
	virtual ~Effect();

	virtual void Init() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(ID2D1RenderTarget* renderTarget) override;

	virtual RenderLayer GetRenderLayer() override;
	bool IsBlockingCell() override { return false; }

private:
};

