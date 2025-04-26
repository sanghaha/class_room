#include "pch.h"
#include "Player.h"
#include "InputManager.h"
#include "GameScene.h"
#include "ResourceManager.h"
#include "BitmapTexture.h"
#include "Enemy.h"
#include "Game.h"
#include "PlayerState.h"

Player::Player(Vector pos) : Super(pos)
{
}

Player::~Player()
{
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


	// 애니메이션 정보
	_animInfo[PlayerAnimType::PA_IDLE] = AnimInfo(0, 0, 6, 1, true, 0.6f);
	_animInfo[PlayerAnimType::PA_MOVE] = AnimInfo(0, 1, 6, 1, true, 0.6f);
	_animInfo[PlayerAnimType::PA_ATTACK_SIDE] = AnimInfo(0, 2, 6, 1, false, 0.6f);
	_animInfo[PlayerAnimType::PA_ATTACK_DOWN] = AnimInfo(0, 4, 6, 1, false, 0.6f);
	_animInfo[PlayerAnimType::PA_ATTACK_UP] = AnimInfo(0, 6, 6, 1, false, 0.6f);


	// state 정보
	_stateMachine.AddState(new PlayerState_Idle(this));
	_stateMachine.AddState(new PlayerState_Move(this));
	_stateMachine.AddState(new PlayerState_Attack(this));

	_stateMachine.SetDefaultState((int32)PlayerStateType::S_IDLE);
	_stateMachine.ReserveNextState((int32)PlayerStateType::S_IDLE);
}

void Player::Update(float deltaTime)
{
	Super::Update(deltaTime);
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

bool Player::Move(int32 dirX, int32 dirY)
{
	bool result = Super::Move(dirX, dirY);
	if (result)
	{
		ChangeState(PlayerStateType::S_MOVE);
	}

	return result;
}

void Player::takeDamage()
{
	--_hp;

	if (_hp <= 0)
	{
		Game::GetGameScene()->ReserveRemove(this);
	}
}
