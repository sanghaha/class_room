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
	void Render(ID2D1RenderTarget* renderTarget) override;
	void CreateGrid() override;
	bool OnLeftClickEvent(int32 x, int32 y) override;

	// 갈수 있는 타일인지 판단
	bool CanMove(Cell cell);
	bool FindPath(Cell start, Cell end, deque<Cell>& findPath, int32 maxDepth = 10);

	void CreateExplosionEffect(Vector pos);
	void CreateDropItem(Vector pos, int32 itemId);
	void CreateArrow(Vector pos, DirType dir, Cell dest, int32 attak);

	class Map* GetMap() { return _map; }
	class Player* GetPlayer() { return _player; }


	int32 GetCurrStage() { return _currStage; }
	void CreateStage(int32 stage);

	void ShowGameOver();
	void ToggleVisibleInventory();

protected:
	void loadResources() override;
	void createObjects() override;
	void createUI() override;

	void initTimer() override;
private:
	class Map*		_map = nullptr;
	class Player*	_player = nullptr;
	int32		_currStage = 0;

	// UI
	class UIMessage*			_uiMsg = nullptr;
	class UIHud*				_uiHud = nullptr;
	class UIInventory*			_uiInven = nullptr;
};

