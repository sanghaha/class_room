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

Player::Player(Pos pos, wstring bitmapKey) : Super(pos, bitmapKey, true)
{
}

Player::~Player()
{
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

	//Pos renderPos = GetPos();
	//float bottom = Game::GetGameScene()->GetCameraPos().y + GWinSizeY / 2;
	//bottom -= (GetCollider()->GetRadius() * 2.f);
	// �÷��̾ ī�޶� ���� ��ǥ��� ȭ���� ������� ���´�.
	//if (bottom < GetPos().y)
	//{
	//	renderPos.y = bottom;
	//	SetPos(renderPos);
	//}
}

void Player::Init()
{
	Super::Init();

	// std::bind�� ����Ͽ� ��� �Լ� ���
	//_collider.SetEnterCollisionCallback((CollisionFunc)(std::bind(&Player::OnEnterCollision, this, std::placeholders::_1, std::placeholders::_2)));
	//_collider.SetExitCollisionCallback((CollisionFunc)(std::bind(&Player::OnExitCollision, this, std::placeholders::_1, std::placeholders::_2)));
	//_collider.SetOverlapCollisionCallback((CollisionFunc)(std::bind(&Player::OnOverlapCollision, this, std::placeholders::_1, std::placeholders::_2)));

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

void Player::Render(HDC hdc)
{
	Super::Render(hdc);
}

void Player::OnEnterCollision(ColliderCircle* src, ColliderCircle* other)
{
	// other �浹ü�� �Ѿ� �϶� HP ����
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
	//--_hp;

	// ������ ����Ʈ ���
	Game::GetGameScene()->CreateExplosion(GetPos());

	if (_hp <= 0)
	{
		Destroy();
	}
}

void Player::move(float x, float y)
{
	Pos curPos = GetPos();
	if (curPos.x + x < 0 - _size.w || curPos.x + x >= Game::GetScene()->GetMapSize().w - _size.w ||
		curPos.y + y < 0 - _size.h || curPos.y + y >= Game::GetScene()->GetMapSize().h - _size.h)
	{
		// ����
	}
	else
	{
		AddPosDelta(x, y);
	}
}
