#pragma once

class BaseResource;

class Actor
{
public:
	Actor(Vector pos, bool convertCellPos = true);
	virtual ~Actor();

	virtual void Init();
	virtual void Update(float deltaTime);
	virtual void Render(ID2D1RenderTarget* renderTarget);
	void Destory();

	virtual RenderLayer GetRenderLayer() abstract;
	virtual bool IsBlockingCell() { return false; }

	virtual void OnBeginOverlapActor(Actor* other) {}
	virtual void OnEndOverlapActor(Actor* other) {}

	virtual bool OnLeftClickEvent(int32 x, int32 y) { return false; }

	Vector GetPos() { return _pos; }
	bool AddPosDelta(float deltaTime);
	bool IsCompleteMove() const { return _moveDeltaDistance <= 0; }
	void SetPos(Vector pos, bool notifyScene = true);
	Cell GetPosCell() const { return _posCell; }
	void SetPosCell(Cell cell, bool notifyScene = true);

	float GetMoveSpeed() const { return _moveSpeed; }
	void SetMoveSpeed(float speed) { _moveSpeed = speed; }

protected:
	bool IsCulling();

	// Actor 
	class Sprite* CreateSpriteComponent(string spriteName, int32 width = 0, int32 height = 0);
	class Texture* CreateTextureComponent(string bitmapKey, int32 width = 0, int32 height = 0);

private:
	Vector _pos = {};
	Cell _posCell;	// ������ ��ġ�� �ִ� Cell�� ( _pos ���� Cell�̵��߿� �ǽð����� ǥ���ϴ� ��ǥ��)
	float _moveDeltaDistance = 0; // �̵��߿� �󸶳� �̵��ߴ��� üũ�ϴ� ��

	vector<BaseResource*> _components;
	float			_moveSpeed = 300;
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


protected:
	Sprite* _sprite = nullptr;
	Rect _collision;
};

enum AnimType
{
	A_IDLE,
	A_MOVE,
	A_ATTACK,
	A_DEAD,
	A_MAX
};

class AnimSpriteActor : public SpriteActor
{
	using Super = SpriteActor;

public:
	AnimSpriteActor(Vector pos);
	virtual ~AnimSpriteActor();

	void Update(float deltaTime) override;
	void SetAnimInfo(const AnimInfo& info);
	bool IsEndAnimation() { return _isEnd; }

protected:
	int32 _animIndexX = 0;
	int32 _animIndexY = 0;
	bool _isEnd = false;
	float _sumTime = 0;

	AnimInfo _anim;
};