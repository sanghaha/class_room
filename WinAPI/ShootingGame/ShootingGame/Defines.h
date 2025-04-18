#pragma once

const int32 GWinSizeX = 480;
const int32 GWinSizeY = 800;

const float PI = 3.1415926f;

struct Size
{
	int32 w = 0;
	int32 h = 0;
};

struct Pos
{
	float x = 0;
	float y = 0;
};

struct Dir
{
	float xDir = 0;
	float yDir = 0;
};

enum RenderLayer
{
	RL_Background,
	RL_Enemy,
	RL_Bullet,
	RL_Player,
	RL_Effect,
	RL_Count
};