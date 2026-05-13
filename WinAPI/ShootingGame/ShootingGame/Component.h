#pragma once

class Component
{
public:
	Component();
	virtual ~Component();


	virtual void UpdateComponent(float deltaTime) {}
	virtual void RenderComponent(HDC hdc, Pos pos) {}

protected:

};

