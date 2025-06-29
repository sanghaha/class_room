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

	HDC		_textureHdc = 0;
	HBITMAP _bitmap;
	int32 _sizeX = 0;
	int32 _sizeY = 0;
	int32 _transparent = -1;
	//class LineMesh* _lineMesh = nullptr;
};

