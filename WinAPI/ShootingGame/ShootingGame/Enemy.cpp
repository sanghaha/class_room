#include "pch.h"
#include "Enemy.h"
#include "ColliderCircle.h"
#include "Bullet.h"
#include "GameScene.h"
#include "Game.h"

Enemy::Enemy(Pos pos, wstring bitmapKey, int32 bulletIndex) : Super(pos, bitmapKey, false), _bulletIndex(bulletIndex)
{
}

Enemy::~Enemy()
{
}

void Enemy::Update(float deltaTime)
{
	if (_collider == nullptr)
		return;

	Super::Update(deltaTime);

	// ���� �������� �Ѿ��� �߻�
	_sumTime += deltaTime;

	if (_sumTime > _shootTime)
	{
		// �Ѿ� �߻�
		Pos pos = GetCenterPos();
		pos.y += _collider->GetRadius() + 10;
		Game::GetGameScene()->CreateEnemyBullet(pos, _bulletIndex);

		_sumTime -= _shootTime;
	}

	// �¿�� �����̸� ������ �����´�.
	float x = _moveSpeedX * deltaTime * sinf(_sumRadian);
	float y = _moveSpeedY * deltaTime;
	AddPosDelta(x, y);

	_sumRadian += (_turnSpeed * deltaTime);

	if (GetPos().y > Game::GetScene()->GetMapSize().h)
	{
		// ȭ�� ������ ������ ���� ����
		Destroy();
	}
}

void Enemy::Init()
{
	Super::Init();

	_collider->SetEnterCollisionCallback(
		[this](ColliderCircle* src, ColliderCircle* other) {
			this->OnEnterCollision(src, other);
		}
	);

	_collider->SetExitCollisionCallback(
		[this](ColliderCircle* src, ColliderCircle* other) {
			this->OnExitCollision(src, other);
		}
	);

	_collider->SetOverlapCollisionCallback(
		[this](ColliderCircle* src, ColliderCircle* other) {
			this->OnOverlapCollision(src, other);
		}
	);
}

void Enemy::OnEnterCollision(ColliderCircle* src, ColliderCircle* other)
{
	// other�� �Ѿ��̸� ���� ����
	Bullet* bullet = dynamic_cast<Bullet*>(other->GetOnwer());
	if (bullet && bullet->GetBulletType() == BulletType::BT_Player)
	{
		Destroy();

		// ������ ����Ʈ ���
		Game::GetGameScene()->CreateExplosion(GetPos());

		// ���ھ� ����
		Game::GetGameScene()->AddScore(100);
	}
}

void Enemy::OnExitCollision(ColliderCircle* src, ColliderCircle* other)
{
}

void Enemy::OnOverlapCollision(ColliderCircle* src, ColliderCircle* other)
{
}
