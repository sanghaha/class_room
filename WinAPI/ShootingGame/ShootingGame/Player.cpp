#include "pch.h"
#include "Player.h"
#include "InputManager.h"
#include "GameScene.h"
#include "Bullet.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "Enemy.h"
#include "Game.h"

Player::Player(Pos pos) : Super(pos)
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

	if (InputManager::GetInstance()->GetButtonUp(KeyType::SpaceBar))
	{
		Game::GetGameScene()->CreatePlayerBullet(GetPos());
	}

	// hp 이미지 표시
	_hpTexture.Update(deltaTime);
}

void Player::Init()
{
	Super::Init();

	// std::bind를 사용하여 멤버 함수 등록
	//_collider.SetEnterCollisionCallback((CollisionFunc)(std::bind(&Player::OnEnterCollision, this, std::placeholders::_1, std::placeholders::_2)));
	//_collider.SetExitCollisionCallback((CollisionFunc)(std::bind(&Player::OnExitCollision, this, std::placeholders::_1, std::placeholders::_2)));
	//_collider.SetOverlapCollisionCallback((CollisionFunc)(std::bind(&Player::OnOverlapCollision, this, std::placeholders::_1, std::placeholders::_2)));

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

	Sprite* sprite = ResourceManager::GetInstance()->GetSprite(L"Explosion");
	_hpTexture.SetSprite(sprite, 0.01f);
	_hpTexture.SetEnd();	// 시작은 끝나도록 설정해서 안보이게 한다
}

void Player::Render(HDC hdc)
{
	Super::Render(hdc);

	if (_hpTexture.IsEnd() == false)
	{
		_hpTexture.Render(hdc, GetPos());
	}
}

void Player::OnEnterCollision(ColliderCircle* src, ColliderCircle* other)
{
	// other 충돌체가 총알 일때 HP 감소
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

	_hpTexture.Reset();

	if (_hp <= 0)
	{
		Game::GetGameScene()->ReserveRemove(this);
	}
}

void Player::move(float x, float y)
{
	Pos curPos = GetPos();
	if (curPos.x + x < 0 || curPos.x + x >= GWinSizeX - GetSize().w || curPos.y + y < 0 || curPos.y + y >= GWinSizeY - GetSize().h)
	{
		// 못감
	}
	else
	{
		AddPosDelta(x, y);
	}
}
