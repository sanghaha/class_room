#pragma once

class Actor
{
public:
	Actor(Pos pos);
	virtual ~Actor();

	virtual void Init() abstract;
	virtual void Update(float deltaTime) abstract;
	virtual void Render(HDC hdc) abstract;

	virtual RenderLayer GetRenderLayer() abstract;
	virtual class ColliderCircle* GetCollider() abstract;

	Pos GetPos() { return _pos; }

protected:
	Pos _pos = {};
};

