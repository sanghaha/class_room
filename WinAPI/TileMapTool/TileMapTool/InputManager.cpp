#include "pch.h"
#include "InputManager.h"

void InputManager::Init(HWND hwndMain, HWND hwndSub)
{
	_hwndMain = hwndMain;
	_hwndSub = hwndSub;
	_states.resize(KEY_TYPE_COUNT, KeyState::None);
}

void InputManager::Update()
{
	BYTE asciiKeys[KEY_TYPE_COUNT] = {};
	if (::GetKeyboardState(asciiKeys) == false)
		return;

	for (uint32 key = 0; key < KEY_TYPE_COUNT; key++)
	{
		// 키가 눌려 있으면 true
		if (asciiKeys[key] & 0x80)
		{
			KeyState& state = _states[key];

			// 이전 프레임에 키를 누른 상태라면 PRESS
			if (state == KeyState::Press || state == KeyState::Down)
				state = KeyState::Press;
			else
				state = KeyState::Down;
		}
		else
		{
			KeyState& state = _states[key];

			// 이전 프레임에 키를 누른 상태라면 UP
			if (state == KeyState::Press || state == KeyState::Down)
				state = KeyState::Up;
			else
				state = KeyState::None;
		}
	}

	// Mouse
	::GetCursorPos(&_mainMousePos); // 커서의 좌표를 알아온다
	::ScreenToClient(_hwndMain, &_mainMousePos);

	::GetCursorPos(&_subMousePos); // 커서의 좌표를 알아온다
	::ScreenToClient(_hwndSub, &_subMousePos);
}
