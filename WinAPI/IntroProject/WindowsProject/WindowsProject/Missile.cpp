#include "pch.h"
#include "Missile.h"
#include "Game.h"
#include "GameScene.h"
#include "Enemy.h"

void Missile::Init(Vector pos, float angle)
{
	_pos = pos;
	_angle = angle;
}

void Missile::Update(float deltaTime)
{
	if (_target == nullptr)
	{
		_sumTime += deltaTime;

		_pos.x += (_speed * ::cos(_angle) * deltaTime);
		_pos.y -= (_speed * ::sin(_angle) * deltaTime);

		if (_sumTime >= _chaseTime)
		{
			GameScene* gameScene = dynamic_cast<GameScene*>(Game::GetInstance()->GetScene());
			if (gameScene)
			{
				_target = gameScene->GetEnemy();
			}
		}
	}
	else
	{
		// 타겟을 향해 날아가자.
		 Vector dir =_target->GetPos() - _pos;
		 dir.Normalize();

		 _pos += (dir* _speed * deltaTime);
	}



	// 소멸
	if (_pos.x < 0 || _pos.x >= GWinSizeX ||
		_pos.y < 0 || _pos.y >= GWinSizeY)
	{
		GameScene* gameScene = dynamic_cast<GameScene*>(Game::GetInstance()->GetScene());
		if (gameScene)
		{
			gameScene->RemoveMissile(this);
		}
		return;
	}
}

void Missile::Render(HDC hdc)
{
	Ellipse(hdc, _pos.x- 10, _pos.y- 10, _pos.x + 10, _pos.y + 10);
}
