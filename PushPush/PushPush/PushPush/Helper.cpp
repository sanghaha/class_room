#include <Windows.h>
#include "Helper.h"

MoveDir GMoveDir = MoveDir::MOVE_MAX;
bool GGameOver = false;

// 이건 그냥 포장한거다. 함수의 재사용. 이렇게 함수로 안묶으면 매번 3줄을 코드로 쳐야한다.
void SetCursorPosition(int x, int y)
{
	HANDLE output = ::GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { (SHORT)x, (SHORT)y };
	::SetConsoleCursorPosition(output, pos);
}

void SetCursorOnOff(bool visible)
{
	HANDLE output = ::GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	::GetConsoleCursorInfo(output, &cursorInfo);
	cursorInfo.bVisible = visible;
	::SetConsoleCursorInfo(output, &cursorInfo);
}

void SetCursorColor(Color color, Color bg)
{
	int code = (bg << 4) | color;
	//int code = color;
	// 색상코드: 배경색(4bit) + 글자색(4bit) 조합
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, code);
}

void HandleKeyInput()
{
	if (::GetAsyncKeyState(VK_LEFT) & 0x01)
		GMoveDir = MOVE_LEFT;
	else if (::GetAsyncKeyState(VK_RIGHT) & 0x01)
		GMoveDir = MOVE_RIGHT;
	else if (::GetAsyncKeyState(VK_UP) & 0x01)
		GMoveDir = MOVE_UP;
	else if (::GetAsyncKeyState(VK_DOWN) & 0x01)
		GMoveDir = MOVE_DOWN;
	else
		GMoveDir = MOVE_MAX;
}
