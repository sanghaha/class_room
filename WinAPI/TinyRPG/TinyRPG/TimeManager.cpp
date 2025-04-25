#include "pch.h"
#include "TimeManager.h"

int32 TimeManager::TimerIdGenerator = 0;

void TimeManager::Init()
{
	::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&_frequency));
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&_prevCount)); // CPU 클럭
}

void TimeManager::Update()
{
	uint64 currentCount;
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currentCount));

	_deltaTime = (currentCount - _prevCount) / static_cast<float>(_frequency);
	_prevCount = currentCount;

	_frameCount++;
	_frameTime += _deltaTime;

	// 초당 프레임률 계산
	if (_frameTime >= 1.0f)
	{
		_fps = _frameCount;
		_frameTime -= 1.0f;
		_frameCount = 0;
	}

	// 타이머 호출
	for (auto& iter : _timers)
	{
		iter.second.Update(_deltaTime);
	}
}

void TimeManager::AddTimer(Timer&& timer)
{
	int32 id = TimerIdGenerator++;
	if (_timers.find(id) != _timers.end())
	{
		// 이미 존재하는 키라면 리턴
		return;
	}

	_timers.emplace(id, std::move(timer));
}

void TimeManager::Remove(int32 id)
{
	if (_timers.find(id) != _timers.end())
	{
		_timers.erase(id);
	}
}

void Timer::Update(float deltaTime)
{
	_sumTime += deltaTime;
	if (_sumTime >= _interval)
	{
		_func();
		_sumTime -= _interval;
	}
}
