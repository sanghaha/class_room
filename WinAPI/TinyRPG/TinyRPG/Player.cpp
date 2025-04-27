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
	_attack = 5;
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
	{
		AnimInfo info = AnimInfo(0, 0, 6, 1, true, 0.6f);
		for (int32 i = 0; i < DirType::DIR_MAX; ++i)
		{
			_animInfo[AnimType::A_IDLE][i] = info;
		}
		_animInfo[AnimType::A_IDLE][DirType::DIR_LEFT].FlipX = -1;
	}
	{
		AnimInfo info = AnimInfo(0, 1, 6, 1, true, 0.6f);
		for (int32 i = 0; i < DirType::DIR_MAX; ++i)
		{
			_animInfo[AnimType::A_MOVE][i] = info;
		}
		_animInfo[AnimType::A_MOVE][DirType::DIR_LEFT].FlipX = -1;
	}
	{
		_animInfo[AnimType::A_ATTACK][DirType::DIR_RIGHT] = AnimInfo(0, 2, 6, 1, false, 0.6f);
		_animInfo[AnimType::A_ATTACK][DirType::DIR_LEFT] = AnimInfo(0, 2, 6, 1, false, 0.6f, -1);
		_animInfo[AnimType::A_ATTACK][DirType::DIR_DOWN] = AnimInfo(0, 4, 6, 1, false, 0.6f);
		_animInfo[AnimType::A_ATTACK][DirType::DIR_UP] = AnimInfo(0, 6, 6, 1, false, 0.6f);
	}


	// state 정보
	_stateMachine.AddState(new PlayerState_Idle(this));
	_stateMachine.AddState(new PlayerState_Move(this));
	_stateMachine.AddState(new PlayerState_Attack(this));

	_stateMachine.SetDefaultState((int32)PlayerStateType::PS_IDLE);
	_stateMachine.ReserveNextState((int32)PlayerStateType::PS_IDLE);
}

void Player::Update(float deltaTime)
{
	int32 moveX = InputManager::GetInstance()->GetMoveDirX();
	int32 moveY = InputManager::GetInstance()->GetMoveDirY();

	if (moveX != 0)
	{
		_currDir = moveX > 0 ? DirType::DIR_RIGHT : DirType::DIR_LEFT;
	}
	if (moveY != 0)
	{
		_currDir = moveY > 0 ? DirType::DIR_DOWN : DirType::DIR_UP;
	}

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
		ChangeState(PlayerStateType::PS_MOVE);
	}

	return result;
}