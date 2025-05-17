#include "Player.h"
#include "Map.h"
#include "Helper.h"

int GPlayerX = 2;
int GPlayerY = 2;

void MovePlayer(int x, int y)
{
	if (x < 0 || x >= MAP_SIZE)
		return;
	if (y < 0 || y >= MAP_SIZE)
		return;

	//int index = (y * MAP_SIZE) + x;
	if (GMap1D[x][y] == ObjectType::WALL)
		return; // ���� ������

	if (GMap1D[x][y] == ObjectType::Goal)
		return; // ��ǥ ������ ������

	if (GMap1D[x][y] == ObjectType::Box)
	{
		// �ڽ��� ������ �������� �о�� �Ѵ�.

	}

	// ���� ��ġ ����
	{
		GMap1D[GPlayerY][GPlayerX] = 0;
	}

	GPlayerX = x;
	GPlayerY = y;

	// �� ��ġ �̵�
	{
		int index = (GPlayerY * MAP_SIZE) + GPlayerX;
		GMap1D[GPlayerY][GPlayerX] = ObjectType::PLAYER;
	}
}

void HandleMove()
{
	if (GMoveDir == MoveDir::MOVE_LEFT)
	{
		MovePlayer(GPlayerX - 1, GPlayerY);
	}
	else if (GMoveDir == MoveDir::MOVE_RIGHT)
	{
		MovePlayer(GPlayerX + 1, GPlayerY);
	}
	else if (GMoveDir == MoveDir::MOVE_UP)
	{
		MovePlayer(GPlayerX, GPlayerY - 1);
	}
	else if (GMoveDir == MoveDir::MOVE_DOWN)
	{
		MovePlayer(GPlayerX, GPlayerY + 1);
	}
}