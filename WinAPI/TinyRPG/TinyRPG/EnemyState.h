#pragma once
#include "State.h"


enum EnemyStateType
{
	ES_IDLE,
	ES_PATROL,	// 랜덤 위치로 이동
	ES_CHASE,	// 플레이어 추적
	ES_RETURN,	// 원래 위치로 복귀
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
	int32 _chaseCount = 5;
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
	float _idleTime = 2.0f;
};


class EnemyState_Patrol : public EnemyState
{
	using Super = EnemyState;
public:
	EnemyState_Patrol(class Enemy* enemy);
	virtual ~EnemyState_Patrol();

	int32 GetStateType() override { return EnemyStateType::ES_PATROL; }
	AnimType GetAnimType() override;
	void Enter() override;
	void Update(float deltaTime)override;
	bool IsEnd() override;

private:

private:
	Cell			_startCell;
	int8			_range = 2;

	int8			_dirX = 0;
	int8			_dirY = 0;
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

private:
	void calcChasePath();

private:
	std::vector<Cell> _path;
	Cell _destCell;
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
	float _attackTime = 0;
};

class EnemyState_Return : public EnemyState
{
	using Super = EnemyState;
public:
	EnemyState_Return(class Enemy* enemy);
	virtual ~EnemyState_Return();

	int32 GetStateType() override { return EnemyStateType::ES_RETURN; }
	AnimType GetAnimType() override;
	void Enter() override;
	void Update(float deltaTime)override;
	bool IsEnd() override;

private:
	std::vector<Cell> _path;
};
