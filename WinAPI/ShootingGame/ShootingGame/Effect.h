#pragma once

#include "Actor.h"

class Effect : public Actor
{
	using Super = Actor;
public:
	Effect();
	virtual ~Effect();

	using Super::Init;
	void Init(Pos pos, wstring bitmapKey, float frameTime);
	void Update(float deltaTime) override;
	void Render(HDC hdc) override;
	RenderLayer GetRenderLayer() override { return RenderLayer::RL_Effect; }
	class ColliderCircle* GetCollider() override { return nullptr; }

private:
	class SpriteRenderer* _spriteRenderer = nullptr;
};
