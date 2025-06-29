#pragma once

class Missile
{
public:

	void Init(Vector pos, float angle);
	void Update(float deltaTime);
	void Render(HDC hdc);

private:
	Vector _pos;
	float _speed = 500;
	float _angle = 0;
};

