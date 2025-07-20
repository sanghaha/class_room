#pragma once

template<typename T>
class Singleton
{
public:
	static T* GetInstance()
	{
		if (_instance == nullptr)
		{
			_instance = new T();
		}
		return _instance;
	}
	static void DestroyInstance()
	{
		if (_instance != nullptr)
		{
			_instance->Destroy();
			delete _instance;
			_instance = nullptr;
		}
	}

protected:
	// 생성자와 소멸자를 protected로 설정하여 외부에서 직접 생성/삭제를 방지
	Singleton() = default;
	virtual ~Singleton() = default;

	// 복사 및 이동 금지
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
	Singleton(Singleton&&) = delete;
	Singleton& operator=(Singleton&&) = delete;

	virtual void Destroy() {}

private:
	static T* _instance;
};

// 정적 멤버 초기화
template<typename T>
T* Singleton<T>::_instance = nullptr;