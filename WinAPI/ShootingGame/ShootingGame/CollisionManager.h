#pragma once
#include "Singleton.h"

class CollisionManager : public Singleton<CollisionManager>
{
public:
	CollisionManager();
	virtual ~CollisionManager();

	void Init();
	void Update();

private:
	void addCollisionState(class ColliderCircle* src, class ColliderCircle* other);
	void removeCollisionState(class ColliderCircle* src, class ColliderCircle* other);

private:
	// key : scr Collider, value : other Collider List
	set<tuple<class ColliderCircle*, class ColliderCircle*>>	_collisionMap;
};

