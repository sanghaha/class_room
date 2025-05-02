#pragma once
#include "Actor.h"
#include "AnimSprite.h"

class EffectExplosion : public Actor
{
	using Super = Actor;
public:
	EffectExplosion(Vector pos);
	virtual ~EffectExplosion();

	virtual void Init() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(ID2D1HwndRenderTarget* renderTarget) override;

	virtual RenderLayer GetRenderLayer() override;
	virtual class ColliderCircle* GetCollider() override;

private:
	AnimSprite		_renderer;
	AnimInfo		_animInfo;
};

