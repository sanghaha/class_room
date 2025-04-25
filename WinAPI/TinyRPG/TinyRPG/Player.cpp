#include "pch.h"
#include "Player.h"
#include "InputManager.h"
#include "GameScene.h"
#include "ResourceManager.h"
#include "BitmapTexture.h"
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

	bool pressedMoveKey = false;
	if (InputManager::GetInstance()->GetButtonPressed(KeyType::W))
	{
		pressedMoveKey = true;
		move(0, -_moveSpeed * deltaTime);
	}
	if (InputManager::GetInstance()->GetButtonPressed(KeyType::S))
	{
		pressedMoveKey = true;
		move(0, _moveSpeed * deltaTime);
	}
	if (InputManager::GetInstance()->GetButtonPressed(KeyType::A))
	{
		pressedMoveKey = true;
		move(-_moveSpeed * deltaTime, 0);
	}
	if (InputManager::GetInstance()->GetButtonPressed(KeyType::D))
	{
		pressedMoveKey = true;
		move(_moveSpeed * deltaTime, 0);
	}
	if (InputManager::GetInstance()->GetButtonUp(KeyType::SpaceBar))
	{
		changeState(PlayerState::P_SIDE_ATTACK);
	}

	if(pressedMoveKey == false)
	{
		changeState(PlayerState::P_IDLE);
	}
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

	_animInfo[PlayerState::P_IDLE] = AnimInfo(0, 0, 6, 1, true, 0.6f);
	_animInfo[PlayerState::P_WALK] = AnimInfo(0, 1, 6, 1, true, 0.6f);
	_animInfo[PlayerState::P_SIDE_ATTACK] = AnimInfo(0, 2, 6, 1, false, 0.6f);
	_animInfo[PlayerState::P_DOWN_ATTACK] = AnimInfo(0, 4, 6, 1, false, 0.6f);
	_animInfo[PlayerState::P_UP_ATTACk] = AnimInfo(0, 6, 6, 1, false, 0.6f);
}

void Player::Render(HDC hdc)
{
	Super::Render(hdc);
}

void Player::OnEnterCollision(ColliderCircle* src, ColliderCircle* other)
{

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

	if (_hp <= 0)
	{
		Game::GetGameScene()->ReserveRemove(this);
	}
}

void Player::move(float x, float y)
{
	Pos curPos = GetPos();
	//if (curPos.x + x < 0 || curPos.x + x >= GWinSizeX - GetSize().Width || curPos.y + y < 0 || curPos.y + y >= GWinSizeY - GetSize().Height)
	//{
	//	// 못감
	//}
	//else
	{
		AddPosDelta(x, y);

		_dirX = x < 0 ? -1 : 1;
		changeState(PlayerState::P_WALK);
	}
}

void Player::changeState(PlayerState state)
{
	if (_curState == state)
		return;

	_renderer.SetAnimInfo(&_animInfo[state]);
	_curState = state;
}
