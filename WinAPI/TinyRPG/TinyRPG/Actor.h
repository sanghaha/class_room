#pragma once

class Actor
{
public:
	Actor(Vector pos);
	virtual ~Actor();

	virtual void Init();
	virtual void Update(float deltaTime) abstract;
	virtual void Render(HDC hdc) abstract;

	virtual RenderLayer GetRenderLayer() abstract;
	virtual class ColliderCircle* GetCollider() abstract;

	Vector GetPos() { return _pos; }
	virtual void AddPosDelta(float x, float y, bool notifyScene = true);
	void SetPos(Vector pos, bool notifyScene = true);
	Cell GetPosCell() const { return _posCell; }
	void SetPosCell(Cell cell) { _posCell = cell; }
private:
	Vector _pos = {};
	Cell _posCell;	// ������ ��ġ�� �ִ� Cell�� ( _pos ���� Cell�̵��߿� �ǽð����� ǥ���ϴ� ��ǥ��)
};