#pragma once

#include "Actor.h"

class ObjectPool
{
public:
    ObjectPool()
    {

    }
    virtual ~ObjectPool();

    template<typename T>
    void Init(int32 size)
    {
        for (int32 i = 0; i < size; ++i)
        {
            // 처음에 미리 만들어둠
            T* obj = new T();
            obj->SetObjectPool(this);
            _pool.push_back(obj);
        }
    }

    template<typename T>
    T* Acquire()
    {
        if (_pool.empty())
        {
            // 필요하면 더 만들어서 줌
            T* obj = new T();
            obj->SetObjectPool(this);
            return obj;
        }

        T* obj = dynamic_cast<T*>(_pool.back());
        _pool.pop_back();

        return obj;
    }

    void Return(Actor* obj)
    {
        if (nullptr == obj)
            return;

        _pool.push_back(obj);
    }

private:
	std::vector<Actor*> _pool;
};
