#pragma once
#include "Actor.h"
#include "ColliderCircle.h"
#include "AnimSprite.h"

class Creature : public Actor
{
public:
	using Super = Actor;
public:
	Creature(Pos pos);
	virtual ~Creature();

	void Init() override;
	void Update(float deltaTime) override;
	void Render(HDC hdc) override;
	ColliderCircle* GetCollider() override { return &_collider; }

	virtual void SetTexture(Sprite* sprite);
	
protected:
	float _moveSpeed = 300;
	int32 _dirX = 1;

	ColliderCircle _collider;

	AnimSprite _renderer;
};

