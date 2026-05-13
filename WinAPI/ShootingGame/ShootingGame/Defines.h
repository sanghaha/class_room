#pragma once

const int32 GWinSizeX = 480;
const int32 GWinSizeY = 800;

const float PI = 3.1415926f;

#define SAFE_DELETE(p) if(p) { delete p; p = nullptr; }

struct Size
{
	int32 w = 0;
	int32 h = 0;
};

struct Pos
{
	float x = 0;
	float y = 0;

	Pos() = default;
	Pos(float x, float y) : x(x), y(y) {}
	Pos(POINT pt) : x((float)pt.x), y((float)pt.y) {}

	Pos operator+(const Pos& other) const { return Pos(x + other.x, y + other.y); }
	Pos operator-(const Pos& other) const { return Pos(x - other.x, y - other.y); }
	Pos operator*(float scalar) const { return Pos(x * scalar, y * scalar); }
	Pos operator/(float scalar) const { return Pos(x / scalar, y / scalar); }

	void operator+=(const Pos& other) { x += other.x; y += other.y; }
	void operator-=(const Pos& other) { x -= other.x; y -= other.y; }
};

struct Dir
{
	float xDir = 0;
	float yDir = 0;
};

//struct HBitmapInfo
//{
//	HDC			hdc = 0;
//	HBITMAP		bitmap = 0;
//	int32		transparent = 0;
//	Size		size;
//	int32		countX = 0;
//	int32		countY = 0;
//	bool		loop = false;
//};
//
//struct SpriteInfo
//{
//	int32 startX = 0;
//	int32 endY = 0;
//	float dur = 0;
//	bool loop = false;
//	wstring textureKey;
//};


enum RenderLayer
{
	RL_Background,
	RL_Enemy,
	RL_Bullet,
	RL_Player,
	RL_Effect,
	RL_Count
};

enum BulletType
{
	BT_Player,
	BT_Enemy,
	BT_MAX,
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
		if(pos.x < 0 || pos.y < 0)
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