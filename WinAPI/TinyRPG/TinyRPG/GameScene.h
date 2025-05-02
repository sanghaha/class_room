#pragma once
#include "Scene.h"


struct PQNode
{
	PQNode(int32 cost, Cell pos) : cost(cost), pos(pos) {}

	bool operator<(const PQNode& other) const { return cost < other.cost; }
	bool operator>(const PQNode& other) const { return cost > other.cost; }

	int32 cost;
	Cell pos;
};

class GameScene : public Scene
{
	using Super = Scene;
public:
	GameScene();
	virtual ~GameScene();

	void Init() override;
	void Update(float deltaTime) override;
	void Render(ID2D1HwndRenderTarget* renderTarget) override;
	void CreateGrid() override;

	// 갈수 있는 타일인지 판단
	bool CanMove(Cell cell);
	bool FindPath(Cell start, Cell end, vector<Cell>& findPath, int32 maxDepth = 10);

	void CreateExplosionEffect(Vector pos);

protected:
	void loadResources() override;
	void createObjects() override;
	void initTimer() override;
private:
	class Map* _map = nullptr;

};

