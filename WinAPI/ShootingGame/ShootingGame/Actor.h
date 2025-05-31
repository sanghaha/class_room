#pragma once

class Component;

class Actor
{
public:
	Actor(Pos pos);
	virtual ~Actor();

	virtual void Init();
	virtual void Destroy();

	virtual void Update(float deltaTime);
	virtual void Render(HDC hdc);

	virtual RenderLayer GetRenderLayer() abstract;
	virtual class ColliderCircle* GetCollider() abstract;

	Pos GetPos() { return _pos; }
	void AddPosDelta(float x, float y, bool notifyScene = true);
	void SetPos(Pos pos, bool notifyScene = true);

protected:
	// Actor 
	class Sprite* CreateSpriteComponent(wstring bitmapKey, int32 indexX, int32 indexY, int32 width = 0, int32 height = 0);
	class Texture* CreateTextureComponent(wstring bitmapKey, int32 width = 0, int32 height = 0);
	class ColliderCircle* CreateColliderCircleComponent(Size size, bool checkCell);

private:
	Pos _pos = {};
	vector<Component*> _components;
};


class AnimSpriteActor : public Actor
{
	using Super = Actor;

public:
	AnimSpriteActor(Pos pos, wstring bitmapKey, float frameTime);
	virtual ~AnimSpriteActor();

	void Update(float deltaTime) override;

protected:
	Sprite* _sprite = nullptr;

	int32 _animIndexX = 0;
	int32 _animIndexY = 0;
	bool _isEnd = false;
	float _durtaion = 0.2f;
	float _sumTime = 0;
};