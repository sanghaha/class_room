#pragma once
#include "Actor.h"

class SpriteRenderer;

class Ball : public Actor
{
	using Super = Actor;

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
	void checkFloor();
	void applyPhysics(float deltaTime);
	void applyPhysics_Test(float deltaTime);

private:
	Vector velocity = {};       // 공의 속도
	Vector acceleration = {};	// 가속도
	//float gravity = 9.8f * 70;        // 중력 (아래 방향)
	//Vector gravityVec = { 980.f, 0.f};
	Vector gravityVec = { 0.f, 980.f };
	float mass = 1.0f;               // 질량 (고정값)
	float moveForce = 700.0f;        // 힘의 크기
	float xFriction = 100.0f;         // 좌우 마찰 계수
	float xFrictionExp = 3.0f;           // 수평 감쇠 계수 (공기저항)
	float gravityScale = 2.0f;           // 중력 배율 (높을수록 빠른 궤적, 높이는 유지)
	float maxFallSpeed = 1200.f;         // 최대 낙하 속도
	float bounceSpeedY = 450.f;          // 기준 반사 높이 속도 (gravityScale=1 기준)
	float bounceDampX  = 0.5f;           // 반사 시 수평 즉시 감쇠
	float bounceSpeedX = 150.f;          // 수직 벽 반사 시 강제 수평 속도

	bool onGround = false;


	Vector _debug_prePos = {};
	Vector _debug_normal = {};
	Vector _debug_reflect = {};
	Vector _debug_newPos = {};
};

