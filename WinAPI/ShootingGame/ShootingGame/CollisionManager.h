#pragma once
#include "Singleton.h"

class ColliderCircle;

class CollisionManager : public Singleton<CollisionManager>
{
public:
	CollisionManager();
	virtual ~CollisionManager();

	void Init();
	void Update();
	void Render(HDC hdc);
	wstring GetName() override { return L"CollisionManager"; };

	void AddCheckCollider(ColliderCircle* collider);
	void RemoveCheckCollider(ColliderCircle* collider);

private:
	void addCollisionState(ColliderCircle* src, ColliderCircle* other);
	void removeCollisionState(ColliderCircle* src, ColliderCircle* other);

	void checkCellCollision(ColliderCircle* collider, const Cell& cell);

private:
	// key : scr Collider, value : other Collider List
	set<tuple<ColliderCircle*, ColliderCircle*>>	_collisionMap;

	// 충돌체크를 해야하는 콜라이더 모음 : 플레이어 + 플레이어의 총알
	set<ColliderCircle*> _checkCollider;

public:
	bool drawDebugCell = false;
};

