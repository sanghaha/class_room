#pragma once
#include "State.h"
#include "AnimSprite.h"
#include "Player.h"

enum PlayerStateType
{
	S_IDLE,
	S_MOVE,
	S_ATTACK,
	S_MAX
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

	int32 GetStateType() override { return PlayerStateType::S_IDLE; }
	void Enter() override;
	void Update(float deltaTime)override;

private:
};


class PlayerState_Move : public PlayerState
{
	using Super = PlayerState;
public:
	PlayerState_Move(class Player* player);
	virtual ~PlayerState_Move();

	int32 GetStateType() override { return PlayerStateType::S_MOVE; }
	void Enter() override;
	void Update(float deltaTime)override;
	bool IsEnd() override;

private:
	Vector			_destPos;
	Vector			_prevPos;
};

class PlayerState_Attack : public PlayerState
{
	using Super = PlayerState;
public:
	PlayerState_Attack(class Player* player);
	virtual ~PlayerState_Attack();

	int32 GetStateType() override { return PlayerStateType::S_ATTACK; }
	void Enter() override;
	void Update(float deltaTime)override;
	bool IsEnd() override;

private:
	PlayerAnimType _animType;
};