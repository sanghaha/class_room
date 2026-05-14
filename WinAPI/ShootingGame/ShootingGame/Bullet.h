#pragma once
#include "Actor.h"

class ColliderCircle;

class Bullet : public Actor
{
	using Super = Actor;
public:
	Bullet();
	virtual ~Bullet();

	using Super::Init;
	void Init(Pos pos, wstring bitmapKey, int32 indexX, BulletType type);
	void Update(float deltaTime) override;
	void Render(HDC hdc) override;

	void SetDir(Dir dir) { _dir = dir; }

	RenderLayer GetRenderLayer() override { return RenderLayer::RL_Bullet; }
	ColliderCircle* GetCollider() override { return _collider; }
	BulletType GetBulletType() const { return _type; }

private:
	float _moveSpeed = 500.f;
	Dir _dir = {};
	BulletType _type = BulletType::BT_MAX;
	ColliderCircle* _collider = nullptr;
	ImageRenderer* _texture = nullptr;
};
