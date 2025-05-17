#pragma once

void SetCursorPosition(int x, int y);
void SetCursorOnOff(bool visible);


enum MoveDir
{
	MOVE_NONE,
	MOVE_LEFT,
	MOVE_RIGHT,
	MOVE_UP,
	MOVE_DOWN,
};

extern MoveDir GMoveDir;
void HandleKeyInput();