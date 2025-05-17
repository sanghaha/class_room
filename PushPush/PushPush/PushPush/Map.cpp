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
	{1, 0, 0, 4, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 1},
	{1, 1, 1, 1, 1, 1, 1, 1},
};

void PrintMap1D()
{
	SetCursorPosition(0, 0);

	for (int y = 0; y < MAP_SIZE; y++)
	{ 
		for (int x = 0; x < MAP_SIZE; x++)
		{
			SetCursorColor(Color::Default, GGameOver ? Color::SkyBlue : Color::Black);

			// ¡à ¡á ¢¾ ¡Þ ¡ß 
			switch (GMap1D[y][x])
			{
			case ObjectType::EMPTY:
				cout << "¡à";
				break;
			case ObjectType::WALL:
				cout << "¡á";
				break;
			case ObjectType::PLAYER:
				SetCursorColor(Color::Yellow);
				cout << "¢¾";
				break;
			case ObjectType::Box:
				SetCursorColor(Color::RightBlue);
				cout << "¢Ì";
				break;
			case ObjectType::Goal:
				SetCursorColor(Color::RightGreen);
				cout << "¢Ã";
				break;
			}
		}
		cout << endl;
	}
}
