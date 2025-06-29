#pragma once



class Player
{
public:
	void Init();
	void Update(float deltaTime);
	void Render(HDC hdc);

	Vector GetFirePos();

private:
	Vector _playerPos = Vector(300, 300);
	float _speed = 200;

	float _angle = 0;
	float _length = 100;

	class LineMesh* _lineMesh = nullptr;
};

