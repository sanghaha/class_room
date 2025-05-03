#pragma once
#include "Actor.h"
#include "Sprite.h"

class Projectile : public Actor
{
	using Super = Actor;
public:
	Projectile(Vector pos);
	virtual ~Projectile();

	virtual void Init() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(ID2D1HwndRenderTarget* renderTarget) override;

	virtual RenderLayer GetRenderLayer() override;
	bool IsBlockingCell() override { return false; }

	void SetProjectileInfo(DirType dir, Cell dest, int32 attack);
	int32 GetAttack() const { return _attack; }

private:
	SpriteRenderer		_renderer;
	Cell				_dest;
	DirType				_dir;
	int32				_attack;
};

