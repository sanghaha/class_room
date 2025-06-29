#pragma once



class Player
{
public:
	void Init();
	void Update(float deltaTime);
	void Render(HDC hdc);

	Vector GetFirePos();
	Vector GetForward();

private:
	Vector _playerPos = Vector(300, 300);
	float _speed = 200;

	float _angle = 0;
	float _length = 100;
	float _FOV = 0;

	class LineMesh* _lineMesh = nullptr;
};

