#pragma once

using CollisionFunc = std::function<void(class ColliderCircle*, class ColliderCircle*)>;

class ColliderCircle
{
public:
	ColliderCircle();
	virtual ~ColliderCircle();

	void Init(class Actor* owner, Size size, Pos pos);
	bool CheckCollision(ColliderCircle* other);
	void Render(HDC hdc);
	void Update();

	// get
	Pos GetCenterPos() const { return _centerPos; }
	int GetRadius() const { return _radius; }
	class Actor* GetOnwer() const { return _owner; }

	// 충돌 이벤트 등록 함수
	void SetEnterCollisionCallback(CollisionFunc callback) { _funcEnterCollision = callback; }
	void SetExitCollisionCallback(CollisionFunc callback) { _funcExitCollision = callback; }
	void SetOverlapCollisionCallback(CollisionFunc callback) { _funcOverlapCollision = callback; }


	// 충돌 이벤트 호출 함수
	void OnEnterCollision(ColliderCircle* src, ColliderCircle* other) { if (_funcEnterCollision) _funcEnterCollision(src, other); }
	void OnExitCollision(ColliderCircle* src, ColliderCircle* other) { if (_funcExitCollision) _funcExitCollision(src, other); }
	void OnOverlapCollision(ColliderCircle* src, ColliderCircle* other) { if (_funcOverlapCollision) _funcOverlapCollision(src, other); }


private:
	Pos _centerPos = {};
	Size _size = {};
	int32 _radius = 0;

	CollisionFunc	_funcEnterCollision = nullptr;
	CollisionFunc	_funcExitCollision = nullptr;
	CollisionFunc	_funcOverlapCollision = nullptr;

	class Actor* _owner = nullptr;

	static bool drawDebug;
};

