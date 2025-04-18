#include "pch.h"
#include "Enemy.h"
#include "ColliderCircle.h"
#include "Bullet.h"
#include "Scene.h"

Enemy::Enemy(Pos pos) : Super(pos)
{
}

Enemy::~Enemy()
{
}

void Enemy::Update(float deltaTime)
{
	Super::Update(deltaTime);

	// ���� �������� �Ѿ��� �߻�
	_sumTime += deltaTime;

	if (_sumTime > _shootTime)
	{
		// �Ѿ� �߻�
		Pos pos = _collider.GetCenterPos();
		pos.y += _collider.GetRadius() + 10;
		Scene::GetInstance()->CreateEnemyBullet(pos);

		_sumTime -= _shootTime;
	}

	// �¿�� �����̸� ������ �����´�.
	_pos.x += _moveSpeedX * deltaTime * sinf(_sumRadian);
	_pos.y += _moveSpeedY * deltaTime;
	_sumRadian += (_turnSpeed * deltaTime);

	if (_pos.y > GWinSizeY)
	{
		// ȭ�� ������ ������ ���� ����
		Scene::GetInstance()->ReserveRemove(this);
	}
}

void Enemy::Init()
{
	Super::Init();

	_collider.SetEnterCollisionCallback(
		[this](ColliderCircle* src, ColliderCircle* other) {
			this->OnEnterCollision(src, other);
		}
	);

	_collider.SetExitCollisionCallback(
		[this](ColliderCircle* src, ColliderCircle* other) {
			this->OnExitCollision(src, other);
		}
	);

	_collider.SetOverlapCollisionCallback(
		[this](ColliderCircle* src, ColliderCircle* other) {
			this->OnOverlapCollision(src, other);
		}
	);
}

void Enemy::OnEnterCollision(ColliderCircle* src, ColliderCircle* other)
{
	// other�� �Ѿ��̸� ���� ����
	Bullet* bullet = dynamic_cast<Bullet*>(other->GetOnwer());
	if (bullet && bullet->GetBulletType() != BulletType::BT_Enemy)
	{
		Scene::GetInstance()->ReserveRemove(this);

		// ������ ����Ʈ ���
		Scene::GetInstance()->CreateExplosion(_pos);
	}
}

void Enemy::OnExitCollision(ColliderCircle* src, ColliderCircle* other)
{
}

void Enemy::OnOverlapCollision(ColliderCircle* src, ColliderCircle* other)
{
}
