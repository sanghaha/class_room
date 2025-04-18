#pragma once

#include "Actor.h"
#include "AnimSprite.h"

class Effect : public Actor
{
	using Super = Actor;
public:
	Effect(Pos pos);
	virtual ~Effect();

	void Init() override;
	void Update(float deltaTime) override;
	void Render(HDC hdc) override;
	RenderLayer GetRenderLayer() override { return RenderLayer::RL_Effect; }
	class ColliderCircle* GetCollider() override { return nullptr; }

	void SetTexture(class Sprite* texture, float frameTime);

private:
	AnimSprite _renderer;
};

