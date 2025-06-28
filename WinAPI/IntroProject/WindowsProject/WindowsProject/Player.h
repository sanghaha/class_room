#pragma once



class Player
{
public:
	void Update(float deltaTime);
	void Render(HDC hdc);

private:
	Vector _playerPos = Vector(300, 300);
	float _speed = 100;
};

