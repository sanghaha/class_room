#pragma once

class Map;


// ���⿡�� ȭ�鿡 �׷����� ��� ���͵� ����
class Scene
{
public:
	Scene();
	virtual ~Scene();

	virtual void Init();
	virtual void Destory();
	virtual void Update(float deltaTime);
	virtual void Render(HDC hdc);


	void ReserveRemove(class Actor* actor);

	const vector<class Actor*>& GetRenderList(RenderLayer layer) { return _renderList[layer]; }
	
	void UpdateGrid(class Actor* actor, Pos prevPos, Pos nextPos);
	Cell GetCell(const Pos& pos) const
	{
		return Cell::ConvertToCell(pos, _gridSize);
	}
	const GridInfo& GetGridInfo(const Cell& cell);
	int32 GetGridSize() const { return _gridSize; }

protected:
	virtual void loadResources() abstract;
	virtual void createObjects() abstract;
	virtual void initTimer() abstract;

	virtual void addActor(class Actor* actor);
	virtual void removeActor(class Actor* actor);

	void drawGrid(HDC hdc);

protected:
	unordered_set<class Actor*> _actors;

	// ���� ����, list�� �����ϰ�, actor<->list* ���·� �����ص� �ȴ�.
	vector<class Actor*> _renderList[RenderLayer::RL_Count];

	unordered_set<class Actor*> _reserveAdd;
	unordered_set<class Actor*> _reserveRemove;


	int32 _gridSize = 50;
	int32 _gridCountX = 0;
	int32 _gridCountY = 0;
	map<Cell, GridInfo> _grid;

public:
	static bool drawDebugCell;

};

