#pragma once

class Actor
{
public:
	Actor(Vector pos);
	virtual ~Actor();

	virtual void Init();
	virtual void Update(float deltaTime) abstract;
	virtual void Render(ID2D1HwndRenderTarget* renderTarget) abstract;
	void Destory();

	virtual RenderLayer GetRenderLayer() abstract;
	virtual bool IsBlockingCell() abstract;

	virtual void OnBeginOverlapActor(Actor* other) {}
	virtual void OnEndOverlapActor(Actor* other) {}

	Vector GetPos() { return _pos; }
	bool AddPosDelta(float deltaTime);
	void SetPos(Vector pos, bool notifyScene = true);
	Cell GetPosCell() const { return _posCell; }
	void SetPosCell(Cell cell, bool notifyScene = true);

	float GetMoveSpeed() const { return _moveSpeed; }
	void SetMoveSpeed(float speed) { _moveSpeed = speed; }

protected:
	bool IsCulling() const;

private:
	Vector _pos = {};
	Cell _posCell;	// ������ ��ġ�� �ִ� Cell�� ( _pos ���� Cell�̵��߿� �ǽð����� ǥ���ϴ� ��ǥ��)

	float			_moveSpeed = 300;
};