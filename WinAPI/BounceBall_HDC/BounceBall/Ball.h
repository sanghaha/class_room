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
	void Render(HDC renderTarget) override;
	RenderLayer GetRenderLayer() override;
	ActorType GetActorType() override { return ActorType::AT_BALL; }
	float GetRadius();

	bool OnBeginOverlapActor(Actor* other) override;

private:
	void applyPhysics(float deltaTime);

private:
	Vector velocity = {};       // ���� �ӵ�
	Vector acceleration = {};	// ���ӵ�
	//float gravity = 9.8f * 70;        // �߷� (�Ʒ� ����)
	//Vector gravityVec = { 980.f, 0.f};
	Vector gravityVec = { 0.f, 980.f };
	float mass = 1.0f;               // ���� (������)
	float xFriction = 100.0f;         // �¿� ���� ���

	Vector _debug_prePos = {};
	Vector _debug_normal = {};
	Vector _debug_reflect = {};
	Vector _debug_newPos = {};
};

