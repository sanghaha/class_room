#include "pch.h"
#include "Enemy.h"
#include "InputManager.h"

void Enemy::Init()
{
	_pos.x = 400;
	_pos.y = 100;

	_lineStartPos.x = 200;
	_lineStartPos.y = 50;
	_lineEndPos.x = 600;
	_lineEndPos.y = 100;
}

void Enemy::Update(float deltaTime)
{
	POINT mousePos = InputManager::GetInstance()->GetMousePos();
	Vector mouseVec(mousePos);

	Vector lineDir = _lineEndPos - _lineStartPos;
	float maxDist = lineDir.Length();
	lineDir.Normalize();

	Vector dir = mouseVec - _lineStartPos;
	float dist = lineDir.Dot(dir);

	if (dist < 0)
		dist = 0;
	else if (dist >= maxDist)
		dist = maxDist;

	_pos = _lineStartPos + (lineDir * dist);
}

void Enemy::Render(HDC hdc)
{
	Rectangle(hdc, _pos.x - 25, _pos.y - 25, _pos.x + 25, _pos.y + 25);

	// 이 라인을 따라서 적이 움직인다.
	::MoveToEx(hdc, _lineStartPos.x, _lineStartPos.y, nullptr);
	::LineTo(hdc, _lineEndPos.x, _lineEndPos.y);
}
