#include "Player.h"
#include "Map.h"
#include "Helper.h"

int GPlayerX = 2;
int GPlayerY = 2;


void MovePlayer(MoveDir dir)
{
	if (dir == MoveDir::MOVE_MAX)
		return;

	Pos movePos[MoveDir::MOVE_MAX] = 
	{
		{ -1, 0 }, // LEFT
		{ 1, 0 }, // RIGHT
		{ 0, -1 }, // UP
		{ 0, 1 }, // DOWN
	};
	
	int x = GPlayerX + movePos[dir].x;
	int y = GPlayerY + movePos[dir].y;

	if (x < 0 || x >= MAP_SIZE)
		return;
	if (y < 0 || y >= MAP_SIZE)
		return;

	if (GMap1D[y][x] == ObjectType::WALL)
		return; // 벽을 만났다

	if (GMap1D[y][x] == ObjectType::Goal)
		return; // 목표 지점을 만났다

	if (GMap1D[y][x] == ObjectType::Box)
	{
		// 박스는 가려는 방향으로 밀어야 한다.
		int nextBoxX = x + movePos[dir].x;
		int nextBoxY = y + movePos[dir].y;

		if (GMap1D[nextBoxY][nextBoxX] == ObjectType::WALL)
			return;	// 박스가 밀릴수 없다.

		GMap1D[y][x] = ObjectType::EMPTY; // 박스가 밀린다.
		
		if (GMap1D[nextBoxY][nextBoxX] == ObjectType::Goal)
		{
			// 게임 종료
			GGameOver = true;
		}

		GMap1D[nextBoxY][nextBoxX] = ObjectType::Box;
	}

	// 기존 위치 정리
	{
		GMap1D[GPlayerY][GPlayerX] = 0;
	}

	GPlayerY = y;
	GPlayerX = x;


	// 새 위치 이동
	{
		GMap1D[GPlayerY][GPlayerX] = ObjectType::PLAYER;
	}
}

void HandleMove()
{
	if (GGameOver)
		return;

	MovePlayer(GMoveDir);
}