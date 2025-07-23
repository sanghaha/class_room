#pragma once
#include "Actor.h"

class Effect : public Actor
{
	using Super = Actor;

public:
	Effect(Vector pos, string spriteName);
	virtual ~Effect();

	void Init() override;
	void Update(float deltaTime) override;
	void Render(HDC renderTarget) override;

	RenderLayer GetRenderLayer() override { return RenderLayer::RL_Effect; }
	ActorType GetActorType() override { return ActorType::AT_EFFECT; }
	void SetAutoDestroy(bool destroy) { _autoDestroy = destroy; }

private:
	string _spriteName;
	bool _autoDestroy = true;
	class SpriteRenderer* _spriteRenderer = nullptr;
};

