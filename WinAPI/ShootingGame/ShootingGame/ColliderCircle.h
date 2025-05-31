#pragma once

#include "Component.h"

using CollisionFunc = std::function<void(class ColliderCircle*, class ColliderCircle*)>;

class ColliderCircle : public Component
{
	using Super = Component;
public:
	ColliderCircle(bool checkCell);
	virtual ~ColliderCircle();

	void Init(class Actor* owner, Size size, Pos pos, int32 radius);
	bool CheckCollision(ColliderCircle* other);
	void Update(float deltaTime) override;
	void Render(HDC hdc, Pos pos) override;

	// get
	Pos GetCenterPos() const { return _centerPos; }
	int GetRadius() const { return _radius; }
	class Actor* GetOnwer() const { return _owner; }
	bool CheckCell() const { return _checkCell; }

	// 충돌 이벤트 등록 함수
	void SetEnterCollisionCallback(CollisionFunc callback) { _funcEnterCollision = callback; }
	void SetExitCollisionCallback(CollisionFunc callback) { _funcExitCollision = callback; }
	void SetOverlapCollisionCallback(CollisionFunc callback) { _funcOverlapCollision = callback; }


	// 충돌 이벤트 호출 함수
	void OnEnterCollision(ColliderCircle* src, ColliderCircle* other) { if (_funcEnterCollision) _funcEnterCollision(src, other); }
	void OnExitCollision(ColliderCircle* src, ColliderCircle* other) { if (_funcExitCollision) _funcExitCollision(src, other); }
	void OnOverlapCollision(ColliderCircle* src, ColliderCircle* other) { if (_funcOverlapCollision) _funcOverlapCollision(src, other); }


private:
	class Actor* _owner = nullptr;
	Pos _centerPos = {};
	Size _size = {};
	int32 _radius = 0;
	bool _checkCell = false;

	CollisionFunc	_funcEnterCollision = nullptr;
	CollisionFunc	_funcExitCollision = nullptr;
	CollisionFunc	_funcOverlapCollision = nullptr;

public:
	static bool drawDebug;
};

