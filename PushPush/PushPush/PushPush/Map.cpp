#include <iostream>
#include <iomanip>

#include "Map.h"
#include "Helper.h"
using namespace std;

int GMap1D[MAP_SIZE * MAP_SIZE] =
{
	1, 1, 1, 1, 1,
	1, 0, 0, 0, 1,
	1, 0, 2, 0, 1,
	1, 0, 0, 0, 1,
	1, 1, 1, 1, 1,
};

void PrintMap1D()
{
	SetCursorPosition(0, 0);

	for (int i = 0; i < MAP_SIZE * MAP_SIZE; i++)
	{
		switch (GMap1D[i])
		{
		case 0:
			cout << std::setw(2) << "бр";
			break;
		case 1:
			cout << std::setw(2) << "бс";
			break;
		case 2:
			cout << std::setw(2) << "в┬";
			break;
		}

		if ((i + 1) % MAP_SIZE == 0)
			cout << endl;
	}
}
