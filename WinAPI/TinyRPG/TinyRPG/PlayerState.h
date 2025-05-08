#pragma once
#include "State.h"
#include "PathFind.h"

enum PlayerStateType
{
	PS_IDLE,
	PS_MOVE,
	PS_ATTACK,
	PS_MOVE_PATH,
	PS_MAX
};

class PlayerState : public State
{
public:
	PlayerState(class Player* player) : _player(player) {}
	virtual ~PlayerState() {}

protected:
	class Player* _player;
};

class PlayerState_Idle : public PlayerState
{
	using Super = PlayerState;
public:
	PlayerState_Idle(class Player* player);
	virtual ~PlayerState_Idle();

	int32 GetStateType() override { return PlayerStateType::PS_IDLE; }
	AnimType GetAnimType() override;
	void Enter() override;
	void Update(float deltaTime)override;
	bool OnLeftClickEvent(int32 x, int32 y) override;
private:
};


class PlayerState_Move : public PlayerState
{
	using Super = PlayerState;
public:
	PlayerState_Move(class Player* player);
	virtual ~PlayerState_Move();

	int32 GetStateType() override { return PlayerStateType::PS_MOVE; }
	AnimType GetAnimType() override;
	void Enter() override;
	void Update(float deltaTime)override;
	bool IsEnd() override;

private:
	Vector			_destPos;
};

class PlayerState_Attack : public PlayerState
{
	using Super = PlayerState;
public:
	PlayerState_Attack(class Player* player);
	virtual ~PlayerState_Attack();

	int32 GetStateType() override { return PlayerStateType::PS_ATTACK; }
	AnimType GetAnimType() override;
	void Enter() override;
	void Update(float deltaTime)override;
	bool IsEnd() override;

private:
	float _attackTime = 0;
};

class PlayerState_MovePath : public PlayerState
{
	using Super = PlayerState;
public:
	PlayerState_MovePath(class Player* player);
	virtual ~PlayerState_MovePath();

	int32 GetStateType() override { return PlayerStateType::PS_MOVE_PATH; }
	AnimType GetAnimType() override;
	void Enter() override;
	void Update(float deltaTime)override;
	bool IsEnd() override;

private:
	PathFind _pathFind;
	
};