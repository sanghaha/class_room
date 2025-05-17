#include <Windows.h>
#include "Helper.h"


// �̰� �׳� �����ѰŴ�. �Լ��� ����. �̷��� �Լ��� �ȹ����� �Ź� 3���� �ڵ�� �ľ��Ѵ�.
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

MoveDir GMoveDir;

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
		GMoveDir = MOVE_NONE;
}
