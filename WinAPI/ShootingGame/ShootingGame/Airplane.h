#pragma once
#include "Actor.h"
#include "ColliderCircle.h"

class Texture;
class Airplane : public Actor
{
public:
	using Super = Actor;
public:
	Airplane(Pos pos, wstring bitmapKey, bool checkCell);
	virtual ~Airplane();

	void Init() override;
	void Update(float deltaTime) override;
	void Render(HDC hdc) override;
	ColliderCircle* GetCollider() override { return _collider; }

protected:
	Pos GetCenterPos();

protected:
	float _moveSpeed = 300;
	Size _size;
	ColliderCircle* _collider = nullptr;
};

