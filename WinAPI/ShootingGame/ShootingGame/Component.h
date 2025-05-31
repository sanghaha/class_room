#pragma once

class Component
{
public:
	Component();
	virtual ~Component();


	virtual void Update(float deltaTime) {}
	virtual void Render(HDC hdc, Pos pos) {}

protected:

};

