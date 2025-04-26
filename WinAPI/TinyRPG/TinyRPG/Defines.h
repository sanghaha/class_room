#pragma once

const int32 GWinSizeX = 480;
const int32 GWinSizeY = 600;

const int32 GTileSize = 64;

const float PI = 3.1415926f;


struct Vector
{
	float x = 0;
	float y = 0;

	Vector operator+(const Vector& other)
	{
		Vector ret;
		ret.x = x + other.x;
		ret.y = y + other.y;
		return ret;
	}

	Vector operator-(const Vector& other)
	{
		Vector ret;
		ret.x = x - other.x;
		ret.y = y - other.y;
		return ret;
	}

	Vector operator*(float value)
	{
		Vector ret;
		ret.x = x * value;
		ret.y = y * value;
		return ret;
	}

	void operator+=(const Vector& other)
	{
		x += other.x;
		y += other.y;
	}

	void operator-=(const Vector& other)
	{
		x -= other.x;
		y -= other.y;
	}

	void operator*=(float ratio)
	{
		x *= ratio;
		y *= ratio;
	}

	float LengthSquared()
	{
		return x * x + y * y;
	}

	float Length()
	{
		return ::sqrt(LengthSquared());
	}

	float Dot(Vector other)
	{
		return x * other.x + y * other.y;
	}

	float Cross(Vector other)
	{
		return x * other.y - y * other.x;
	}

	void Normalize()
	{
		float length = Length();
		if (length < 0.00000000001f)
			return;

		x /= length;
		y /= length;
	}
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


// 2차원 그리드로 관리
struct GridInfo
{
	set<class Actor*> _actors;
	bool canMoveCell = false;
};


// 셀의 위치를 나타내는 구조체
struct Cell
{
	int32 index_X;
	int32 index_Y;

	static Cell ConvertToCell(Vector pos)
	{
		if (pos.x < 0 || pos.y < 0)
			return Cell{ -1, -1 };
		return Cell{ (int32)(pos.x / GTileSize), (int32)(pos.y / GTileSize) };
	}
	Vector ConvertToPos()
	{
		return Vector{ (float)(index_X * GTileSize) + GTileSize/2, (float)(index_Y * GTileSize) + GTileSize / 2 };
	}

	bool operator==(const Cell& other) const
	{
		return index_X == other.index_X && index_Y == other.index_Y;
	}

	// '<' 연산자 정의
	bool operator<(const Cell& other) const
	{
		if (index_X != other.index_X)
			return index_X < other.index_X;
		return index_Y < other.index_Y;
	}
};

void PrintLog(wstring log);