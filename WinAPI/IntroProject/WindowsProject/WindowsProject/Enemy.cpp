#include "pch.h"
#include "Enemy.h"

void Enemy::Init()
{
	_pos.x = 400;
	_pos.y = 100;
}

void Enemy::Update(float deltaTime)
{
}

void Enemy::Render(HDC hdc)
{
	Rectangle(hdc, _pos.x, _pos.y, _pos.x + 50, _pos.y + 50);
}
