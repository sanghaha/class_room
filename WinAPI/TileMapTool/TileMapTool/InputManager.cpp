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
		// Ű�� ���� ������ true
		if (asciiKeys[key] & 0x80)
		{
			KeyState& state = _states[key];

			// ���� �����ӿ� Ű�� ���� ���¶�� PRESS
			if (state == KeyState::Press || state == KeyState::Down)
				state = KeyState::Press;
			else
				state = KeyState::Down;
		}
		else
		{
			KeyState& state = _states[key];

			// ���� �����ӿ� Ű�� ���� ���¶�� UP
			if (state == KeyState::Press || state == KeyState::Down)
				state = KeyState::Up;
			else
				state = KeyState::None;
		}
	}

	// Mouse
	::GetCursorPos(&_mainMousePos); // Ŀ���� ��ǥ�� �˾ƿ´�
	::ScreenToClient(_hwndMain, &_mainMousePos);

	::GetCursorPos(&_subMousePos); // Ŀ���� ��ǥ�� �˾ƿ´�
	::ScreenToClient(_hwndSub, &_subMousePos);
}
