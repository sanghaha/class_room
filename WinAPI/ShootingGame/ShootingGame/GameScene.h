#pragma once
#include "Scene.h"

class GameScene : public Scene
{
	using Super = Scene;
public:
	GameScene();
	virtual ~GameScene();

	void Init() override;
	void Update(float deltaTime) override;
	void Render(HDC hdc) override;

	void CreatePlayerBullet(Pos pos);
	void CreateEnemyBullet(Pos pos, int32 bulletIndex);
	void CreateExplosion(Pos pos);
	void CreateRandomEnemy();

	int32 GetScore() { return _scroe; }
	void AddScore(int32 score) { _scroe += score; }

	class Player* GetPlayer() { return _player; }

protected:
	void loadResources() override;
	void createObjects() override;
	void initTimer() override;

	void addActor(class Actor* actor) override;
	void removeActor(class Actor* actor) override;

private:
	class Player* _player = nullptr;
	int32 _scroe = 0;
};

