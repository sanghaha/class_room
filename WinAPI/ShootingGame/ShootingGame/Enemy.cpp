#include "pch.h"
#include "Enemy.h"
#include "ColliderCircle.h"
#include "Bullet.h"
#include "GameScene.h"
#include "Game.h"

Enemy::Enemy(Pos pos) : Super(pos)
{
}

Enemy::~Enemy()
{
}

void Enemy::Update(float deltaTime)
{
	Super::Update(deltaTime);

	// 일정 간격으로 총알을 발사
	_sumTime += deltaTime;

	if (_sumTime > _shootTime)
	{
		// 총알 발사
		Pos pos = _collider.GetCenterPos();
		pos.y += _collider.GetRadius() + 10;
		Game::GetGameScene()->CreateEnemyBullet(pos);

		_sumTime -= _shootTime;
	}

	// 좌우로 움직이며 밑으로 내려온다.
	float x = _moveSpeedX * deltaTime * sinf(_sumRadian);
	float y = _moveSpeedY * deltaTime;
	AddPosDelta(x, y);

	_sumRadian += (_turnSpeed * deltaTime);

	if (GetPos().y > GWinSizeY)
	{
		// 화면 밖으로 나가면 삭제 예약
		Game::GetGameScene()->ReserveRemove(this);
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
	// other이 총알이면 삭제 예약
	Bullet* bullet = dynamic_cast<Bullet*>(other->GetOnwer());
	if (bullet && bullet->GetBulletType() == BulletType::BT_Player)
	{
		Game::GetGameScene()->ReserveRemove(this);

		// 터지는 이펙트 재생
		Game::GetGameScene()->CreateExplosion(GetPos());

		// 스코어 증가
		Game::GetGameScene()->AddScore(100);
	}
}

void Enemy::OnExitCollision(ColliderCircle* src, ColliderCircle* other)
{
}

void Enemy::OnOverlapCollision(ColliderCircle* src, ColliderCircle* other)
{
}
