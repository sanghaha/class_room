#pragma once



class Actor;

class ObjectPool
{
public:
    ObjectPool()
    {

    }
    virtual ~ObjectPool();

    template<typename T>
    void Init(int32 size, std::function<T* ()> factory)
    {
        for (int32 i = 0; i < size; ++i)
        {
            // 처음에 미리 만들어줌
            T* obj = factory();
            _pool.push_back(obj);

            //wstring text = std::format(L"Init Obj : {0}\n", (void*)obj);
            //OutputDebugString(text.c_str());
        }
    }

    template<typename T>
    T* Acquire(std::function<T*()> factory)
    {
        if (_pool.empty())
        {
            // 필요하면 더 만들어도 됨
            T* obj = factory();

            //wstring text = std::format(L"Extra New Obj : {0}\n", (void*)obj);
            //OutputDebugString(text.c_str());
            return obj;
        }

        T* obj = dynamic_cast<T*>(_pool.back());
        _pool.pop_back();

        //wstring text = std::format(L"Get Obj : {0}\n", (void*)obj);
        //OutputDebugString(text.c_str());
        return obj;
    }

    void Return(Actor* obj)
    {
        if (nullptr == obj)
            return;

        _pool.push_back(obj);

        //wstring text = std::format(L"Return Obj : {0}\n", (void*)obj);
        //OutputDebugString(text.c_str());
    }

private:
	std::vector<Actor*> _pool;
};

