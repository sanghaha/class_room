#pragma once
#include "Scene.h"

class GameScene : public Scene
{
	using Super = Scene;
public:
	GameScene();
	virtual ~GameScene();

	void Init() override;
	void Render(HDC hdc) override;

	void CreatePlayerBullet(Pos pos);
	void CreateEnemyBullet(Pos pos);
	void CreateExplosion(Pos pos);
	void CreateRandomEnemy();

	int32 GetScore() { return _scroe; }
	void AddScore(int32 score) { _scroe += score; }

protected:
	void loadResources() override;
	void createObjects() override;
	void initTimer() override;
private:
	int32 _scroe = 0;
};

