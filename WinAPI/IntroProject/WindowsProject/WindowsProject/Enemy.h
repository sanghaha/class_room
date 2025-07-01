#pragma once

class Enemy
{
public:
	void Init();
	void Update(float deltaTime);
	void Render(HDC hdc);

	Vector GetPos() { return _pos; }

private:
	Vector _pos;

	Vector _lineStartPos;
	Vector _lineEndPos;
};

