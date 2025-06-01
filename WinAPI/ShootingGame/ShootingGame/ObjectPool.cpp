#include "pch.h"
#include "ObjectPool.h"
#include "Actor.h"
#include "Bullet.h"

ObjectPool::~ObjectPool()
{
	for (int32 i = 0; i < _pool.size(); ++i)
	{
		SAFE_DELETE(_pool[i]);
	}
	_pool.clear();
}
