#pragma once

class Enemy
{
public:
	void Init();
	void Update(float deltaTime);
	void Render(HDC hdc);

private:
	Vector _pos;
};

