#pragma once

struct PQNode
{
	//bool operator<(const PQNode& other) const { return cost < other.cost; }
	bool operator>(const PQNode& other) const
	{
		int f = g + h;
		int otherF = other.g + other.h;

		if (f == otherF)
			return g > other.g;  // f�� ���ٸ�, ������� ��� (g) ���� �������� ����.

		return f > otherF;
	}

	int g;
	int h;
	Cell pos;
};

class DebugAStar
{
public:
	void Update(float deltaTime);
	void Render(ID2D1RenderTarget* renderTarget);


	map<Cell, int32> best;
	map<Cell, Cell> parent;
	set<Cell> closedList;
	map<Cell, PQNode> openList;

	bool _drawDebug = false;
};

