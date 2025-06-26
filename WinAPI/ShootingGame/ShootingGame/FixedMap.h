#pragma once
#include "Actor.h"

class Texture;

class FixedMap : public Actor
{
	using Super = Actor;
public:
	FixedMap(Pos pos);
	virtual ~FixedMap();

	void Init() override;
	void Update(float deltaTime) override;
	void Render(HDC hdc) override;
	RenderLayer GetRenderLayer() override { return RenderLayer::RL_Background; }
	class ColliderCircle* GetCollider() override { return nullptr; }

	Size GetMapSize();
private:
	Texture* _texture = nullptr;
	Size _textureSize = {};	// 텍스쳐의 크기
};

