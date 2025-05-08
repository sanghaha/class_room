#pragma once
#include "Creature.h"

class Player : public Creature
{
	using Super = Creature;
public:
	Player(Vector pos);
	virtual ~Player();

	void Init() override;
	void Update(float deltaTime) override;
	void Render(ID2D1RenderTarget* renderTarget) override;
	RenderLayer GetRenderLayer() override { return RenderLayer::RL_Player; }
	void OnBeginOverlapActor(Actor* other) override;
	void OnEndOverlapActor(Actor* other) override;
	bool OnLeftClickEvent(int32 x, int32 y) override;

	bool Move(int32 dirX, int32 dirY) override;
	void ChangeWeapon(WeaponType type);

	void ResetAnimation(AnimType type) override;
	bool CanAttackToTarget(Creature* target) override;
	void Attack() override;

	float GetAttackTime();
protected:
	void OnDead() override;
	AnimInfo* calcDirAnim(AnimType type) override;

private:
	// 무기 타입별 애니메이션
	struct WeaponTypeAnim
	{
		AnimInfo		_animInfo[AnimType::A_MAX][DirType::DIR_MAX];
	};

	WeaponTypeAnim*		_currAnimInfo = nullptr;
	WeaponTypeAnim		_swordAnim;
	WeaponTypeAnim		_bowAnim;
};

