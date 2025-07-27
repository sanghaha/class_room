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
	void Render(ID2D1RenderTarget* renderTarget) override;
	RenderLayer GetRenderLayer() override { return RenderLayer::RL_Enemy; }

	void TurnToPlayerDir(class Player* player);
	
	Cell GetSpawnedCell() const { return _spawnedCell; }
	const MonsterData* GetData() const { return _data; }

	bool CanAttackToTarget(Creature* target) override;
	void OnBeginOverlapActor(Actor* other) override;

protected:
	void OnDead() override;
	AnimInfo* calcDirAnim(AnimType type) override;
private:
	Cell _spawnedCell;
	const MonsterData* _data = nullptr;
	AnimInfo		_animInfo[AnimType::A_MAX][DirType::DIR_MAX];
};

