#include <iostream>
#include <iomanip>

#include "Map.h"
#include "Helper.h"
using namespace std;



int GMap1D[MAP_SIZE][MAP_SIZE] =
{
	{1, 1, 1, 1, 1, 1, 1, 1},
	{1, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 2, 0, 3, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 4, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 1},
	{1, 1, 1, 1, 1, 1, 1, 1},
};

void PrintMap1D()
{
	SetCursorPosition(0, 0);

	for (int i = 0; i < MAP_SIZE; i++)
	{ 
		for (int j = 0; j < MAP_SIZE; j++)
		{
			// �� �� �� �� �� 
			switch (GMap1D[i][j])
			{
			case ObjectType::EMPTY:
				cout << "��";
				break;
			case ObjectType::WALL:
				cout << "��";
				break;
			case ObjectType::PLAYER:
				cout << "��";
				break;
			case ObjectType::Box:
				cout << "��";
				break;
			case ObjectType::Goal:
				cout << "��";
				break;
			}
		}
		cout << endl;
	}
}
