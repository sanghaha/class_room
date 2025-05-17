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

	int index = (y * MAP_SIZE) + x;
	if (GMap1D[index] == 1)
		return; // ���� ������

	// ���� ��ġ ����
	{
		int prevIndex = (GPlayerY * MAP_SIZE) + GPlayerX;
		GMap1D[prevIndex] = 0;
	}

	GPlayerX = x;
	GPlayerY = y;

	// �� ��ġ �̵�
	{
		int index = (GPlayerY * MAP_SIZE) + GPlayerX;
		GMap1D[index] = 2;
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