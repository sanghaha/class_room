#pragma once

class Actor
{
public:
	Actor(Pos pos);
	virtual ~Actor();

	virtual void Init();
	virtual void Update(float deltaTime) abstract;
	virtual void Render(HDC hdc) abstract;

	virtual RenderLayer GetRenderLayer() abstract;
	virtual class ColliderCircle* GetCollider() abstract;

	Pos GetPos() { return _pos; }
	void AddPosDelta(float x, float y, bool notifyScene = true);
	void SetPos(Pos pos, bool notifyScene = true);

private:
	Pos _pos = {};
};

