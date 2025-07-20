#pragma once

const int32 GWinSizeX = 1250;
const int32 GWinSizeY = 750;

const int32 BLOCK_SIZE = 64;
const int32 BALL_SIZE = 30;

#define SAFE_RELEASE(p) if(p) { p->Release(); p = nullptr; }
#define SAFE_DELETE(p) if(p) { delete p; p = nullptr; }

enum RenderLayer
{
	RL_Background,
	RL_Object,
	RL_Ball,
	RL_Effect,
	RL_Count
};

//struct Size
//{
//	int32 Width = 0;
//	int32 Height = 0;
//};

struct SpriteInfo
{
	int32 indexX = 0;
	int32 indexY = 0;
	wstring bitmapKey;
	bool alignCenter = true;
};

enum BrushColor
{
	Red,
	White,
	Blue,
	Green,
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

	Vector() : x(0), y(0) {} 
	Vector(float x_, float y_) : x(x_), y(y_) {}
	Vector(int32 x_, int32 y_) : x((float)x_), y((float)y_) {}

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

	Vector operator-(const Vector& other) const
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

	Vector operator-() const
	{
		return Vector{ -x, -y };
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

	Vector GetNormalize()
	{
		float length = Length();
		if (length < 0.00000000001f)
			return *this;

		Vector normalize = *this;
		normalize.Normalize();
		return normalize;
	}
};

// 2차원 그리드로 관리
struct GridInfo
{
	set<class Actor*> _actors;
};

// 셀의 위치를 나타내는 구조체
struct Cell
{
	int32 index_X;
	int32 index_Y;

	static Cell ConvertToCell(Vector pos, int32 gridSize)
	{
		if (pos.x < 0 || pos.y < 0)
			return Cell{ -1, -1 };
		return Cell{ (int32)(pos.x / gridSize), (int32)(pos.y / gridSize) };
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

struct MyRect
{
	float    left;
	float    top;
	float    right;
	float    bottom;
};

void PrintLog(wstring log);

//min <= 결과값 <= max
int32 RandRange(int32 min, int32 max);

// point 체크
bool IsInPoint(RECT rect, POINT pos);
bool CheckCircleAABB(float circleX, float circleY, float radius, MyRect rect, Vector& outNormal, Vector& outPos);
bool LineIntersectsAABB(Vector p0, Vector p1, const MyRect& rect, Vector& outNormal, Vector& outPos);
bool IntersectSegmentRect(const Vector& A, const Vector& B, const MyRect& r, Vector& outNormal, Vector& outPos);