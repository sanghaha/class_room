#pragma once
#include "Actor.h"
#include "ColliderCircle.h"

class Texture;
class Airplane : public Actor
{
public:
	using Super = Actor;
public:
	Airplane(Pos pos);
	virtual ~Airplane();

	void Init() override;
	void Update(float deltaTime) override;
	void Render(HDC hdc) override;
	ColliderCircle* GetCollider() override { return &_collider; }

	void SetTexture(Texture* texture);
	Size GetSize();
protected:
	float _moveSpeed = 300;
	Texture* _texture = nullptr;
	ColliderCircle _collider;
};

