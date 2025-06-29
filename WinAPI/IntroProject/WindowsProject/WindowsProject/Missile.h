#pragma once

class Missile
{
public:

	void Init(Vector pos, float angle);
	void Update(float deltaTime);
	void Render(HDC hdc);

private:
	Vector _pos;
	float _speed = 400;
	float _angle = 0;

	float _chaseTime = 0.3f;
	float _sumTime = 0;

	class Enemy* _target = nullptr;
};

