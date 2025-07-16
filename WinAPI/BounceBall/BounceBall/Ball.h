#pragma once
#include "Actor.h"

class Sprite;

class Ball : public SpriteActor
{
	using Super = SpriteActor;

public:
	Ball(Vector pos);
	virtual ~Ball();

	void Init() override;
	void Update(float deltaTime) override;
	void Render(ID2D1RenderTarget* renderTarget) override;
	RenderLayer GetRenderLayer() override;
	ActorType GetActorType() override { return ActorType::AT_BALL; }
	float GetRadius();

	bool OnBeginOverlapActor(Actor* other) override;

private:
	void applyPhysics(float deltaTime);

private:
	Vector velocity = {};       // ���� �ӵ�
	Vector acceleration = {};	// ���ӵ�
	float gravity = 9.8f * 70;        // �߷� (�Ʒ� ����)

	Vector _debug_prePos = {};
	Vector _debug_normal = {};
	Vector _debug_reflect = {};
	Vector _debug_newPos = {};
};

