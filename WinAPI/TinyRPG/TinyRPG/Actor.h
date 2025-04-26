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
	Cell _posCell;	// 실제로 위치해 있는 Cell값 ( _pos 값은 Cell이동중에 실시간으로 표현하는 좌표값)
};