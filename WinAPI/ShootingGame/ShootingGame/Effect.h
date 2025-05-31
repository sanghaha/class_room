#pragma once

#include "Actor.h"

class Effect : public AnimSpriteActor
{
	using Super = AnimSpriteActor;
public:
	Effect(Pos pos, wstring bitmapKey, float frameTime);
	virtual ~Effect();

	void Init() override;
	void Update(float deltaTime) override;
	void Render(HDC hdc) override;
	RenderLayer GetRenderLayer() override { return RenderLayer::RL_Effect; }
	class ColliderCircle* GetCollider() override { return nullptr; }

private:
};

