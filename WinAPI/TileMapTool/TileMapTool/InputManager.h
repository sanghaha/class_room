#pragma once
#include "Singleton.h"

enum class KeyType
{
	LeftMouse = VK_LBUTTON,
	RightMouse = VK_RBUTTON,

	Up = VK_UP,
	Down = VK_DOWN,
	Left = VK_LEFT,
	Right = VK_RIGHT,
	SpaceBar = VK_SPACE,

	KEY_1 = '1',
	KEY_2 = '2',

	W = 'W',
	A = 'A',
	S = 'S',
	D = 'D',
	L = 'L',
	Q = 'Q',
	E = 'E',

	F1 = VK_F1,
};

enum class KeyState
{
	None,
	Press,
	Down,
	Up,

	End
};

enum
{
	KEY_TYPE_COUNT = static_cast<int32>(UINT8_MAX) + 1,
	KEY_STATE_COUNT = static_cast<int32>(KeyState::End)
};

class InputManager : public Singleton<InputManager>
{
public:
	void Init(HWND hwndMain, HWND hwndSub);
	void Update();

	// 누르고 있을 때
	bool GetButtonPressed(KeyType key) { return GetState(key) == KeyState::Press; }

	// 맨 처음 눌렀을 때
	bool GetButtonDown(KeyType key) { return GetState(key) == KeyState::Down; }

	// 맨 처음 눌렀다가 땔 때
	bool GetButtonUp(KeyType key) { return GetState(key) == KeyState::Up; }

	POINT GetMainMousePos() { return _mainMousePos; }
	POINT GetSubMousePos() { return _subMousePos; }

private:
	KeyState GetState(KeyType key) { return _states[static_cast<uint8>(key)]; }

private:
	HWND _hwndMain = 0;
	HWND _hwndSub = 0;

	vector<KeyState> _states;
	POINT _mainMousePos = {};
	POINT _subMousePos = {};
};

