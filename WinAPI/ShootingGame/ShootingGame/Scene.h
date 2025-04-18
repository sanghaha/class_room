#pragma once

#include "Singleton.h"

class Map;

// ���⿡�� ȭ�鿡 �׷����� ��� ���͵� ����
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

private:
	void loadResources();
	void createObjects();

	void addActor(class Actor* actor);
	void removeActor(class Actor* actor);

private:
	unordered_set<class Actor*> _actors;
	vector<class Actor*> _renderList[RenderLayer::RL_Count];

	vector<class Actor*> _reserveAdd;
	vector<class Actor*> _reserveRemove;
};

