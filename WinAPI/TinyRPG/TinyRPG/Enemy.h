#pragma once
#include "Creature.h"
class MonsterData;

class Enemy : public Creature
{
	using Super = Creature;
public:
	Enemy(const MonsterData* data, Vector pos);
	virtual ~Enemy();

	void Init() override;
	void Update(float deltaTime) override;
	void Render(ID2D1HwndRenderTarget* renderTarget) override;
	RenderLayer GetRenderLayer() override { return RenderLayer::RL_Enemy; }

	void OnEnterCollision(ColliderCircle* src, ColliderCircle* other);
	void OnExitCollision(ColliderCircle* src, ColliderCircle* other);
	void OnOverlapCollision(ColliderCircle* src, ColliderCircle* other);


	bool Move(int32 dirX, int32 dirY) override;
	void TurnToPlayerDir(class Player* player);
	
	Cell GetSpawnedCell() const { return _spawnedCell; }
	const MonsterData* GetData() const { return _data; }

private:
	Cell _spawnedCell;
	const MonsterData* _data = nullptr;
};

