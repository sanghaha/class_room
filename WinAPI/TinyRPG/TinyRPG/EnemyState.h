#pragma once
#include "State.h"


enum EnemyStateType
{
	ES_IDLE,
	ES_WANTER,	// 랜덤 위치로 이동
	ES_CHASE,	// 플레이어 추적
	ES_ATTACK,
	ES_MAX
};

class EnemyState : public State
{
public:
	EnemyState(class Enemy* enemy) : _enemy(enemy) {}
	virtual ~EnemyState() {}

protected:
	class Enemy* _enemy;
};

class EnemyState_Idle : public EnemyState
{
	using Super = EnemyState;
public:
	EnemyState_Idle(class Enemy* enemy);
	virtual ~EnemyState_Idle();

	int32 GetStateType() override { return EnemyStateType::ES_IDLE; }
	AnimType GetAnimType() override;
	void Enter() override;
	void Update(float deltaTime)override;

private:
};


class EnemyState_Wanter : public EnemyState
{
	using Super = EnemyState;
public:
	EnemyState_Wanter(class Enemy* enemy);
	virtual ~EnemyState_Wanter();

	int32 GetStateType() override { return EnemyStateType::ES_WANTER; }
	AnimType GetAnimType() override;
	void Enter() override;
	void Update(float deltaTime)override;
	bool IsEnd() override;

private:
	Vector			_destPos;
};

class EnemyState_Chase : public EnemyState
{
	using Super = EnemyState;
public:
	EnemyState_Chase(class Enemy* enemy);
	virtual ~EnemyState_Chase();

	int32 GetStateType() override { return EnemyStateType::ES_CHASE; }
	AnimType GetAnimType() override;
	void Enter() override;
	void Update(float deltaTime)override;
	bool IsEnd() override;
};

class EnemyState_Attack : public EnemyState
{
	using Super = EnemyState;
public:
	EnemyState_Attack(class Enemy* enemy);
	virtual ~EnemyState_Attack();

	int32 GetStateType() override { return EnemyStateType::ES_ATTACK; }
	AnimType GetAnimType() override;
	void Enter() override;
	void Update(float deltaTime)override;
	bool IsEnd() override;

private:
	AnimType _animType;
};