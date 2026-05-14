#include "pch.h"
#include "Player.h"
#include "InputManager.h"
#include "GameScene.h"
#include "Bullet.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "Enemy.h"
#include "Game.h"
#include "CollisionManager.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::Init(Pos pos, wstring bitmapKey)
{
	Super::Init(pos, bitmapKey, true);

	if (_collider)
	{
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
}

void Player::Update(float deltaTime)
{
	Super::Update(deltaTime);

	if (InputManager::GetInstance()->GetButtonPressed(KeyType::W))
	{
		move(0, -_moveSpeed * deltaTime);
	}

	if (InputManager::GetInstance()->GetButtonPressed(KeyType::S))
	{
		move(0, _moveSpeed * deltaTime);
	}

	if (InputManager::GetInstance()->GetButtonPressed(KeyType::A))
	{
		move(-_moveSpeed * deltaTime, 0);
	}

	if (InputManager::GetInstance()->GetButtonPressed(KeyType::D))
	{
		move(_moveSpeed * deltaTime, 0);
	}

	if (InputManager::GetInstance()->GetButtonDown(KeyType::SpaceBar))
	{
		Game::GetGameScene()->CreatePlayerBullet(GetPos());
	}

	// 카메라 기준 벗어나지 못하도록 수정
	Pos cameraPos = Game::GetInstance()->GetCameraPos();
	float radius = 0.0f;
	
	float left = cameraPos.x - GWinSizeX / 2.0f + radius;
	float right = cameraPos.x + GWinSizeX / 2.0f - radius;
	float top = cameraPos.y - GWinSizeY / 2.0f + radius;
	float bottom = cameraPos.y + GWinSizeY / 2.0f - radius;

	Pos currentPos = GetPos();
	currentPos.x = std::clamp(currentPos.x, left, right);
	currentPos.y = std::clamp(currentPos.y, top, bottom);
	SetPos(currentPos);
}

void Player::Render(HDC hdc)
{
	Super::Render(hdc);
}

void Player::OnEnterCollision(ColliderCircle* src, ColliderCircle* other)
{
	// other 浹ü Ѿ ϶ HP 
	Bullet* bullet = dynamic_cast<Bullet*>(other->GetOnwer());
	if (bullet && bullet->GetBulletType() != BulletType::BT_Player)
	{
		takeDamage();
	}

	Enemy* enemy = dynamic_cast<Enemy*>(other->GetOnwer());
	if (enemy)
	{
		takeDamage();
	}
}

void Player::OnExitCollision(ColliderCircle* src, ColliderCircle* other)
{

}

void Player::OnOverlapCollision(ColliderCircle* src, ColliderCircle* other)
{

}

void Player::takeDamage()
{
	--_hp;

	//  Ʈ 
	Game::GetGameScene()->CreateExplosion(GetPos());

	if (_hp <= 0)
	{
		Destroy();
	}
}

void Player::move(float x, float y)
{
	AddPosDelta(x, y);
}
