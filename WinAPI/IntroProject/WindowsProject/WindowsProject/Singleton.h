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
	// �����ڿ� �Ҹ��ڸ� protected�� �����Ͽ� �ܺο��� ���� ����/������ ����
	Singleton() = default;
	virtual ~Singleton() = default;

	// ���� �� �̵� ����
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
	Singleton(Singleton&&) = delete;
	Singleton& operator=(Singleton&&) = delete;

	virtual void Destroy() {}

private:
	static T* _instance;
};

// ���� ��� �ʱ�ȭ
template<typename T>
T* Singleton<T>::_instance = nullptr;