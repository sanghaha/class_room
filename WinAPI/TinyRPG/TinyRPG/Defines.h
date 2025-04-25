#pragma once

const int32 GWinSizeX = 480;
const int32 GWinSizeY = 600;

const float PI = 3.1415926f;


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
	RL_Projectile,
	RL_Player,
	RL_Effect,
	RL_Count
};


// 2���� �׸���� ����
struct GridInfo
{
	set<class Actor*> _actors;
};


// ���� ��ġ�� ��Ÿ���� ����ü
struct Cell
{
	int32 index_X;
	int32 index_Y;

	static Cell ConvertToCell(Pos pos, int32 gridSize)
	{
		if (pos.x < 0 || pos.y < 0)
			return Cell{ -1, -1 };
		return Cell{ (int32)(pos.x / gridSize), (int32)(pos.y / gridSize) };
	}

	bool operator==(const Cell& other) const
	{
		return index_X == other.index_X && index_Y == other.index_Y;
	}

	// '<' ������ ����
	bool operator<(const Cell& other) const
	{
		if (index_X != other.index_X)
			return index_X < other.index_X;
		return index_Y < other.index_Y;
	}
};