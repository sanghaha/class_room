#pragma once

const int MAP_SIZE = 8;

enum ObjectType
{
	EMPTY = 0,
	WALL = 1,
	PLAYER = 2,
	Box = 3,
	Goal = 4,
};


void PrintMap1D();
extern int GMap1D[MAP_SIZE][MAP_SIZE];