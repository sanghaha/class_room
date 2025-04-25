#pragma once

class Map;


// 여기에서 화면에 그려지는 모든 액터들 관리
class Scene
{
public:
	Scene();
	virtual ~Scene();

	virtual void Init();
	virtual void Update(float deltaTime);
	virtual void Render(HDC hdc);
	
	// Actor Remove
	void ReserveRemove(class Actor* actor);

	// Render
	const vector<class Actor*>& GetRenderList(RenderLayer layer) { return _renderList[layer]; }
	class Player* GetPlayer();

	// Cell
	virtual void CreateGrid() {};
	void UpdateGrid(class Actor* actor, Pos prevPos, Pos nextPos);
	Cell GetCell(const Pos& pos) const
	{
		return Cell::ConvertToCell(pos, GTileSize);
	}
	const GridInfo& GetGridInfo(const Cell& cell);

	// Camera
	Pos GetCameraPos() { return _cameraPos; }
	void SetCameraPos(Pos pos) { _cameraPos = pos; }

protected:
	virtual void loadResources() abstract;
	virtual void createObjects() abstract;
	virtual void initTimer() abstract;

	void addActor(class Actor* actor);
	void removeActor(class Actor* actor);

	void drawGrid(HDC hdc);

protected:
	class Player* _player = nullptr;
	unordered_set<class Actor*> _actors;
	vector<class Actor*> _renderList[RenderLayer::RL_Count];

	unordered_set<class Actor*> _reserveAdd;
	unordered_set<class Actor*> _reserveRemove;

	int32 _gridCountX = 0;
	int32 _gridCountY = 0;
	map<Cell, GridInfo> _grid;


	Pos _cameraPos = { 400, 300 };
};

