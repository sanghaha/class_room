#pragma once

const int32 GWinSizeX = 480;
const int32 GWinSizeY = 600;
const int32 GTileSize = 64;

const int32 MAX_EQUIP_SLOT = 8;
const int32 MAX_INVEN_SLOT = 9;

const float PI = 3.1415926f;

#define SAFE_RELEASE(p) if(p) { p->Release(); p = nullptr; }
#define SAFE_DELETE(p) if(p) { delete p; p = nullptr; }

struct Size
{
	int32 Width = 0;
	int32 Height = 0;
};

enum BrushColor
{
	Red,
	White,
	Blue,
};

enum FontSize
{
	FONT_12 = 12,
	FONT_20 = 20,
	FONT_30 = 30,
};

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

	bool operator==(const Vector& other) const
	{
		return (x == other.x && y == other.y);
	}

	bool operator==(Vector& other)
	{
		return (x == other.x && y == other.y);
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

struct Rect
{
	float    left;
	float    top;
	float    right;
	float    bottom;
};

enum RenderLayer
{
	RL_Background,
	RL_Item,
	RL_Enemy,
	RL_Projectile,
	RL_Player,
	RL_Effect,
	RL_UI,	// UI 는 제일 마지막
	RL_Count
};

enum DirType
{
	DIR_LEFT,
	DIR_RIGHT,
	DIR_UP,
	DIR_DOWN,
	DIR_MAX,
};


// 2차원 그리드로 관리
struct GridInfo
{
	set<class Actor*> _actorsInCell;
	bool canMoveCell = false;
	int32 blockedCount = 0;	// 플레이어나, 적군이 있어서 충돌체크되는 상태
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

	Cell NextCell(DirType type, int32 count = 1)
	{
		if (DirType::DIR_RIGHT == type)
		{
			return Cell{ index_X + count, index_Y };
		}
		if (DirType::DIR_LEFT == type)
		{
			return Cell{ index_X - count, index_Y };
		}
		if (DirType::DIR_DOWN == type)
		{
			return Cell{ index_X, index_Y + count };
		}
		if (DirType::DIR_UP == type)
		{
			return Cell{ index_X, index_Y - count };
		}
		return *this;
	}

	int32 DeltaLength(const Cell& other)
	{
		int32 delta_X = other.index_X - index_X;
		int32 delta_Y = other.index_Y - index_Y;
		return abs(delta_X) + abs(delta_Y);
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

struct SpriteInfo
{
	int32 indexX = 0;
	int32 indexY = 0;
	string bitmapKey;
	bool alignCenter = true;
};

struct AnimInfo
{
	int32 startX = 0;
	int32 startY = 0;
	int32 countX = 0;
	int32 countY = 0;
	bool loop = false;
	float durtaion = 0.2f;
	bool flipX = false;
};


void PrintLog(wstring log);

//min <= 결과값 <= max
int32 RandRange(int32 min, int32 max);

// point 체크
bool IsInPoint(RECT rect, POINT pos);