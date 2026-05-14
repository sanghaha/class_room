#pragma once
#include "Actor.h"

class Texture;
class ImageRenderer;

class FixedMap : public Actor
{
	using Super = Actor;
public:
	FixedMap();
	virtual ~FixedMap();

	using Super::Init;
	void Init(Pos pos);
	void Update(float deltaTime) override;
	void Render(HDC hdc) override;
	RenderLayer GetRenderLayer() override { return RenderLayer::RL_Background; }
	class ColliderCircle* GetCollider() override { return nullptr; }

	Size GetMapSize();
private:
	ImageRenderer* _texture = nullptr;
	Size _textureSize = {};	// 텍스쳐의 크기
};
