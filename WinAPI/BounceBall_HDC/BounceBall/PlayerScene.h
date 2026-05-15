#pragma once
#include "Scene.h"
#include "DebugAStar.h"

class Player;

class PlayerScene : public Scene
{
	using Super = Scene;
public:
	PlayerScene();
	virtual ~PlayerScene();

	void Init() override;
	void Destory() override;
	void Update(float deltaTime) override;
	void Render(HDC renderTarget) override;
	bool FindPath(Cell start, Cell end, vector<Cell>& findPath, int32 maxDepth = 10);

private:
	void loadStage();

	int32 _currStage = 1;

	Player* _player = nullptr;

	Vector _selectorPos;
	bool _showSelector = false;

	// for debug
	DebugAStar _debugAStar;
};
