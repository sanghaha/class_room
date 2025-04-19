#pragma once

#include "Singleton.h"

class Map;


// 여기에서 화면에 그려지는 모든 액터들 관리
class Scene : public Singleton<Scene>
{
public:
	void Init();
	void Update(float deltaTime);
	void Render(HDC hdc);

	void CreatePlayerBullet(Pos pos);
	void CreateEnemyBullet(Pos pos);
	void CreateExplosion(Pos pos);
	void CreateRandomEnemy();

	void ReserveRemove(class Actor* actor);

	const vector<class Actor*>& GetRenderList(RenderLayer layer) { return _renderList[layer]; }
	
	void UpdateGrid(class Actor* actor, Pos prevPos, Pos nextPos);
	Cell GetCell(const Pos& pos) const
	{
		return Cell::ConvertToCell(pos, _gridSize);
	}
	const GridInfo& GetGridInfo(const Cell& cell);

private:
	void loadResources();
	void createObjects();

	void addActor(class Actor* actor);
	void removeActor(class Actor* actor);

	void drawGrid(HDC hdc);

private:
	unordered_set<class Actor*> _actors;
	vector<class Actor*> _renderList[RenderLayer::RL_Count];

	vector<class Actor*> _reserveAdd;
	vector<class Actor*> _reserveRemove;


	int32 _gridSize = 100;
	int32 _gridCountX = 0;
	int32 _gridCountY = 0;
	map<Cell, GridInfo> _grid;
};

