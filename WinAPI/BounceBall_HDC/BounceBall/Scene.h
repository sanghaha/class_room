#pragma once

#include "UIManager.h"

// 여기에서 화면에 그려지는 모든 액터들 관리
class Scene
{
	friend class StageLoader;
public:
	Scene();
	virtual ~Scene();

	virtual void Init();
	virtual void Destory();

	virtual void Update(float deltaTime);
	virtual void Render(HDC renderTarget);
	
	// Actor Remove
	void ReserveRemove(class Actor* actor);
	void ReserveAdd(class Actor* actor);

	// Post Update
	void AddPostUpdateAction(std::function<void()> action);

	// Render
	const vector<class Actor*>& GetRenderList(RenderLayer layer) { return _renderList[layer]; }

	void UpdateGrid(class Actor* actor, Vector prevPos, Vector nextPos);
	Cell GetCell(const Vector& pos) const
	{
		return Cell::ConvertToCell(pos, BLOCK_SIZE);
	}
	const GridInfo& GetGridInfo(const Cell& cell);
	
protected:
	

private:
	void addActor(class Actor* actor);
	void removeActor(class Actor* actor);
	void removeAllActor();


protected:
	unordered_set<class Actor*> _actors;
	vector<class Actor*> _renderList[RenderLayer::RL_Count];

	unordered_set<class Actor*> _reserveAdd;
	unordered_set<class Actor*> _reserveRemove;

	// post update 처리
	std::vector<std::function<void()>> _postUpdateActions;

	int32 _gridCountX = 0;
	int32 _gridCountY = 0;
	map<Cell, GridInfo> _grid;

	UIManager _ui;
};