#pragma once


enum MoveDir
{
	MOVE_LEFT,
	MOVE_RIGHT,
	MOVE_UP,
	MOVE_DOWN,
	MOVE_MAX,
};

enum Color
{
	Black = 0,
	SkyBlue = 3,
	Red = 4,
	Default = 7, // 기본 회색
	Yellow = 6,
	RightGreen = 10,
	RightBlue = 9,
	White = 15,
};

struct Pos
{
	int x;
	int y;
};

extern MoveDir GMoveDir;
extern bool GGameOver;

MoveDir HandleKeyInput();

void SetCursorPosition(int x, int y);
void SetCursorOnOff(bool visible);
void SetCursorColor(Color color, Color bg = Color::Black);
