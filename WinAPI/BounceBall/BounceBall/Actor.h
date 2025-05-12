#pragma once

class BaseResource;

enum ActorType : int8
{
	AT_BLOCK,
	AT_BALL,
	AT_STAR,
	AT_EFFECT,
	AT_BACKGROUND,

};

class Actor
{
public:
	Actor(Vector pos);
	virtual ~Actor();
	void Destroy();


	virtual void Init();
	virtual void Update(float deltaTime);
	virtual void Render(ID2D1RenderTarget* renderTarget);
	virtual RenderLayer GetRenderLayer() abstract;
	virtual ActorType GetActorType() abstract;

	virtual Rect* GetCollisionRect() { return nullptr; }

	virtual bool OnBeginOverlapActor(Actor* other) { return false; }
	virtual bool OnEndOverlapActor(Actor* other) { return false; }

	Vector GetPos() { return _pos; }
	void SetPos(Vector pos, bool notifyScene = true);

	virtual void SaveActor(std::wofstream& file);
	virtual void LoadActor(std::wistringstream& steam);

protected:

	// Actor 
	class Sprite* CreateSpriteComponent(string spriteInfo, int32 width = 0, int32 height = 0);
	class Texture* CreateTextureComponent(wstring bitmapKey, int32 width = 0, int32 height = 0);

private:
	Vector _pos = {};

	vector<BaseResource*> _components;
};

class SpriteActor : public Actor
{
	using Super = Actor;

public:
	SpriteActor(Vector pos);
	virtual ~SpriteActor();

	void Init() override;
	void Update(float deltaTime) override;
	void Render(ID2D1RenderTarget* renderTarget) override;

	void SetSpriteIndex(int32 x, int32 y);
	void SaveActor(std::wofstream& file);
	void LoadActor(std::wistringstream& steam);

protected:
	Sprite* _sprite = nullptr;
	Rect _collision;
};

class AnimSpriteActor : public SpriteActor
{
	using Super = SpriteActor;

public:
	AnimSpriteActor(Vector pos);
	virtual ~AnimSpriteActor();

	void Update(float deltaTime) override;

protected:
	int32 _animIndexX = 0;
	int32 _animIndexY = 0;
	bool _isEnd = false;
	bool _loop = false;
	float _durtaion = 0.2f;
	float _sumTime = 0;
};