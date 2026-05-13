#pragma once

#include "Component.h"

using CollisionFuncAABB = std::function<void(class ColliderAABB*, class ColliderAABB*)>;

class ColliderAABB : public Component
{
	using Super = Component;
public:
	ColliderAABB(bool checkCell);
	virtual ~ColliderAABB();

	void Init(class Actor* owner, Size size, Pos pos, Size rectSize);
	bool CheckCollision(ColliderAABB* other);
	void UpdateComponent(float deltaTime) override;
	void RenderComponent(HDC hdc, Pos pos) override;

	// get
	Pos GetCenterPos() const { return _centerPos; }
	Size GetRectSize() const { return _rectSize; }
	class Actor* GetOnwer() const { return _owner; }
	bool CheckCell() const { return _checkCell; }

	// Collision Callback
	void SetEnterCollisionCallback(CollisionFuncAABB callback) { _funcEnterCollision = callback; }
	void SetExitCollisionCallback(CollisionFuncAABB callback) { _funcExitCollision = callback; }
	void SetOverlapCollisionCallback(CollisionFuncAABB callback) { _funcOverlapCollision = callback; }

	// Collision Callback Invoke
	void OnEnterCollision(ColliderAABB* src, ColliderAABB* other) { if (_funcEnterCollision) _funcEnterCollision(src, other); }
	void OnExitCollision(ColliderAABB* src, ColliderAABB* other) { if (_funcExitCollision) _funcExitCollision(src, other); }
	void OnOverlapCollision(ColliderAABB* src, ColliderAABB* other) { if (_funcOverlapCollision) _funcOverlapCollision(src, other); }

private:
	class Actor* _owner = nullptr;
	Pos _centerPos = {};
	Size _size = {}; // Owner actor's size
	Size _rectSize = {}; // AABB rect size
	bool _checkCell = false;

	CollisionFuncAABB	_funcEnterCollision = nullptr;
	CollisionFuncAABB	_funcExitCollision = nullptr;
	CollisionFuncAABB	_funcOverlapCollision = nullptr;

public:
	static bool drawDebug;
};
