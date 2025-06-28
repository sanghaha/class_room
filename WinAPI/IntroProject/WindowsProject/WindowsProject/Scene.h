#pragma once


class Scene
{
public:
	virtual ~Scene() {}
	
	virtual void Init();
	virtual void Update(float deltaTime);
	virtual void Render(HDC hdc);
};

