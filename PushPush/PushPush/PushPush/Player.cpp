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
		return; // ���� ������

	if (GMap1D[y][x] == ObjectType::Goal)
		return; // ��ǥ ������ ������

	if (GMap1D[y][x] == ObjectType::Box)
	{
		// �ڽ��� ������ �������� �о�� �Ѵ�.
		int nextBoxX = x + movePos[dir].x;
		int nextBoxY = y + movePos[dir].y;

		if (GMap1D[nextBoxY][nextBoxX] == ObjectType::WALL)
			return;	// �ڽ��� �и��� ����.

		GMap1D[y][x] = ObjectType::EMPTY; // �ڽ��� �и���.
		
		if (GMap1D[nextBoxY][nextBoxX] == ObjectType::Goal)
		{
			// ���� ����
			GGameOver = true;
		}

		GMap1D[nextBoxY][nextBoxX] = ObjectType::Box;
	}

	// ���� ��ġ ����
	{
		GMap1D[GPlayerY][GPlayerX] = 0;
	}

	GPlayerY = y;
	GPlayerX = x;


	// �� ��ġ �̵�
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