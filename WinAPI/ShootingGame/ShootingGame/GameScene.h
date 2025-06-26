#pragma once
#include "Scene.h"
#include "ObjectPool.h"

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
	virtual	Size GetMapSize() const override;

protected:
	void loadResources() override;
	void createObjects() override;
	void initTimer() override;

	void addActor(class Actor* actor) override;
	void removeActor(class Actor* actor) override;

protected:
	class Bullet* createEnemyBulletObjectPool();
	class Bullet* createPlayerBulletObjectPool();

private:
	class Player* _player = nullptr;
	class FixedMap* _map = nullptr;
	int32 _scroe = 0;

	ObjectPool _enemyBulletPool;
	ObjectPool _playerBulletPool;
};

