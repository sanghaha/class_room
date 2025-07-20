#include "pch.h"
#include "InputManager.h"
#include "Game.h"

void InputManager::Init(HWND hwnd)
{
	_hwnd = hwnd;
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
	::GetCursorPos(&_mousePos); // Ŀ���� ��ǥ�� �˾ƿ´�
	::ScreenToClient(_hwnd, &_mousePos);
}

int32 InputManager::GetMoveDirX() const
{
	if (InputManager::GetInstance()->GetButtonPressed(KeyType::A))
	{
		return -1;
	}
	if (InputManager::GetInstance()->GetButtonPressed(KeyType::D))
	{
		return 1;
	}
	return 0;
}

int32 InputManager::GetMoveDirY() const
{
	if (InputManager::GetInstance()->GetButtonPressed(KeyType::W))
	{
		return -1;
	}
	if (InputManager::GetInstance()->GetButtonPressed(KeyType::S))
	{
		return 1;
	}
	return 0;
}
