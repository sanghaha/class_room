#pragma once
#include "Airplane.h"

class Enemy : public Airplane
{
	using Super = Airplane;
public:
	Enemy(Pos pos, wstring bitmapKey, int32 bulletIndex);
	virtual ~Enemy();

	void Update(float deltaTime) override;
	void Init() override;
	RenderLayer GetRenderLayer() override { return RenderLayer::RL_Enemy; }
	int32 GetBulletIndex() const { return _bulletIndex; }

	void OnEnterCollision(ColliderCircle* src, ColliderCircle* other);
	void OnExitCollision(ColliderCircle* src, ColliderCircle* other);
	void OnOverlapCollision(ColliderCircle* src, ColliderCircle* other);

private:
	float _shootTime = 1;	// �ش� �������� �Ѿ� ������ �߻�
	float _sumTime = 0;
	float _moveSpeedX = 50;
	float _moveSpeedY = 50;
	float _sumRadian = 0;
	float _turnSpeed = 2;
	int32 _bulletIndex = 0;
};

