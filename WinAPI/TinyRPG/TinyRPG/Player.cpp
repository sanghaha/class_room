#include "pch.h"
#include "Player.h"
#include "InputManager.h"
#include "GameScene.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "Enemy.h"
#include "Game.h"
#include "PlayerState.h"
#include "UIManager.h"
#include "Item.h"
#include "InventorySystem.h"
#include "Portal.h"

Player::Player(Vector pos) : Super(pos)
{
	_attack = 5;
	_hp = 100;
	_maxHp = _hp;


	// 애니메이션 정보
	// 검
	{
		AnimInfo info = AnimInfo{ 0, 0, 6, 1, true, 0.6f};
		for (int32 i = 0; i < DirType::DIR_MAX; ++i)
		{
			_swordAnim._animInfo[AnimType::A_IDLE][i] = info;
		}
		_swordAnim._animInfo[AnimType::A_IDLE][DirType::DIR_LEFT].flipX = true;
	}
	{
		AnimInfo info = AnimInfo(0, 1, 6, 1, true, 0.6f);
		for (int32 i = 0; i < DirType::DIR_MAX; ++i)
		{
			_swordAnim._animInfo[AnimType::A_MOVE][i] = info;
		}
		_swordAnim._animInfo[AnimType::A_MOVE][DirType::DIR_LEFT].flipX = true;
	}
	{
		_swordAnim._animInfo[AnimType::A_ATTACK][DirType::DIR_RIGHT] = AnimInfo(0, 2, 6, 1, false, 0.6f);
		_swordAnim._animInfo[AnimType::A_ATTACK][DirType::DIR_LEFT] = AnimInfo(0, 2, 6, 1, false, 0.6f, true);
		_swordAnim._animInfo[AnimType::A_ATTACK][DirType::DIR_DOWN] = AnimInfo(0, 4, 6, 1, false, 0.6f);
		_swordAnim._animInfo[AnimType::A_ATTACK][DirType::DIR_UP] = AnimInfo(0, 6, 6, 1, false, 0.6f);
	}
	// 활
	{
		AnimInfo info = AnimInfo(0, 0, 6, 1, true, 0.6f);
		for (int32 i = 0; i < DirType::DIR_MAX; ++i)
		{
			_bowAnim._animInfo[AnimType::A_IDLE][i] = info;
		}
		_bowAnim._animInfo[AnimType::A_IDLE][DirType::DIR_LEFT].flipX = true;
	}
	{
		AnimInfo info = AnimInfo(0, 1, 6, 1, true, 0.6f);
		for (int32 i = 0; i < DirType::DIR_MAX; ++i)
		{
			_bowAnim._animInfo[AnimType::A_MOVE][i] = info;
		}
		_bowAnim._animInfo[AnimType::A_MOVE][DirType::DIR_LEFT].flipX = true;
	}
	{
		_bowAnim._animInfo[AnimType::A_ATTACK][DirType::DIR_RIGHT] = AnimInfo(0, 4, 8, 1, false, 0.8f);
		_bowAnim._animInfo[AnimType::A_ATTACK][DirType::DIR_LEFT] = AnimInfo(0, 4, 8, 1, false, 0.8f, true);
		_bowAnim._animInfo[AnimType::A_ATTACK][DirType::DIR_DOWN] = AnimInfo(0, 6, 8, 1, false, 0.8f);
		_bowAnim._animInfo[AnimType::A_ATTACK][DirType::DIR_UP] = AnimInfo(0, 2, 8, 1, false, 0.8f);
	}


	// Sprite 생성
	_sprite = CreateSpriteComponent("Warrior_Blue");
	SetAnimInfo(_swordAnim._animInfo[AnimType::A_IDLE][DirType::DIR_RIGHT]);
}

Player::~Player()
{
}

void Player::Init()
{
	Super::Init();

	// state 정보
	_stateMachine.AddState(new PlayerState_Idle(this));
	_stateMachine.AddState(new PlayerState_Move(this));
	_stateMachine.AddState(new PlayerState_Attack(this));
	_stateMachine.AddState(new PlayerState_MovePath(this));

	_stateMachine.SetDefaultState((int32)PlayerStateType::PS_IDLE);
	_stateMachine.ReserveNextState((int32)PlayerStateType::PS_IDLE);
}

void Player::Update(float deltaTime)
{
	if (InputManager::GetInstance()->GetButtonDown(KeyType::Inventory))
	{
		Game::GetGameScene()->ToggleVisibleInventory();
	}

	Super::Update(deltaTime);
}

void Player::Render(ID2D1RenderTarget* renderTarget)
{
	Super::Render(renderTarget);
}

void Player::OnBeginOverlapActor(Actor* other)
{
	// item 일경우 획득 처리
	if (DropItem* item = dynamic_cast<DropItem*>(other))
	{
		item->PickupItem();
	}

	if (Portal* portal = dynamic_cast<Portal*>(other))
	{
		portal->EnterNextStage();
	}
}

void Player::OnEndOverlapActor(Actor* other)
{
}

bool Player::OnLeftClickEvent(int32 x, int32 y)
{
	return _stateMachine.OnLeftClickEvent(x, y);
}

bool Player::Move(int32 dirX, int32 dirY)
{
	bool result = Super::Move(dirX, dirY);
	return result;
}

void Player::ChangeWeapon(WeaponType type)
{
	if (type == WeaponType::Sword)
	{
		if (_sprite)
		{
			_sprite->SetBitmapKey("Warrior_Blue");
		}
		_currAnimInfo = &_swordAnim;
	}
	else if (type == WeaponType::Bow)
	{
		if (_sprite)
		{
			_sprite->SetBitmapKey("Bow_Blue");
		}
		_currAnimInfo = &_bowAnim;
	}
}

bool Player::CanAttackToTarget(Creature* target)
{
	Enemy* enemy = dynamic_cast<Enemy*>(target);
	return (enemy != nullptr);
}

void Player::Attack()
{
	// 활이면 발사체를 생성
	if (_currAnimInfo == &_bowAnim)
	{
		int32 arrowDist = 5;
		Cell dest = GetPosCell().NextCell(GetCurrDir(), arrowDist);
		Game::GetGameScene()->CreateArrow(GetPos(), GetCurrDir(), dest, GetAttack());
	}
	else
	{
		Super::Attack();
	}
}

float Player::GetAttackTime()
{
	// 활이면 발사체를 생성
	if (_currAnimInfo == &_bowAnim)
	{
		return 0.6f;
	}
	return 0.3f;
}

void Player::OnDead()
{
	//UIManager::GetInstance()->ShowGameOver();
}

AnimInfo* Player::calcDirAnim(AnimType type)
{
	if (nullptr == _currAnimInfo)
		return nullptr;

	return &(_currAnimInfo->_animInfo[type][_currDir]);
}
