#pragma once
#include "Actor.h"
#include "ColliderCircle.h"
#include "AnimSprite.h"

enum BulletType
{
	BT_None,
	BT_Player,
	BT_Enemy,
};

class Bullet : public Actor
{
	using Super = Actor;
public:
	Bullet(Pos pos);
	virtual ~Bullet();

	void Init() override;
	void Update(float deltaTime) override;
	void Render(HDC hdc) override;

	void SetResource(Sprite* texture);
	void SetDir(Dir dir) { _dir = dir; }
	void SetBulletType(BulletType type) { _type = type; }

	RenderLayer GetRenderLayer() override { return RenderLayer::RL_Bullet; }
	ColliderCircle* GetCollider() override { return &_collider; }
	BulletType GetBulletType() const { return _type; }

private:
	float _moveSpeed = 500.f;
	Dir _dir = {};
	BulletType _type = BulletType::BT_None;
	AnimSprite _renderer;
	ColliderCircle _collider;
};

