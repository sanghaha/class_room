#pragma once
#include "Actor.h"

enum BulletType
{
	BT_None,
	BT_Player,
	BT_Enemy,
};

class ColliderCircle;

class Bullet : public AnimSpriteActor
{
	using Super = AnimSpriteActor;
public:
	Bullet(Pos pos, wstring bitmapKey, int32 indexX, BulletType type);
	virtual ~Bullet();

	void Init() override;
	void Update(float deltaTime) override;
	void Render(HDC hdc) override;

	void SetDir(Dir dir) { _dir = dir; }

	RenderLayer GetRenderLayer() override { return RenderLayer::RL_Bullet; }
	ColliderCircle* GetCollider() override { return _collider; }
	BulletType GetBulletType() const { return _type; }

private:
	float _moveSpeed = 500.f;
	Dir _dir = {};
	BulletType _type = BulletType::BT_None;
	ColliderCircle* _collider = nullptr;
};

