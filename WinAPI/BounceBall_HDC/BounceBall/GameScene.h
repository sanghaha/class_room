#pragma once
#include "Scene.h"

class UIPanel;

class GameScene : public Scene
{
	using Super = Scene;
public:
	GameScene();
	virtual ~GameScene();

	void Init() override;
	void Destory() override;
	void Update(float deltaTime) override;
	void Render(HDC renderTarget) override;

	bool CheckCollision(class Ball* ball, Vector start, Vector end, Vector& outNormal, Vector& outPos);
	bool CheckCollision_ClosestPoint(class Ball* ball, Vector ballPos, float radius, Vector& outNormal, Vector& outPos);

	void CreateEffect(Vector pos, string spriteName);
	void AddStarCount(Vector pos);
	void Dead(Vector pos);

protected:
	bool loadStage(int32 stage);
	void clearStage();

	void createCollisionMask();

	void onClickNextStageButton();

private:
	uint64 COLLISION_BIT_MASK_BLOCK = 0;	// Ball 액터와 충돌(block) 체크용 Bit mask
	uint64 COLLISION_BIT_MASK_OVERLAP = 0;	// Ball 액터와 충돌(overlap) 체크용 Bit mask

	int32 _currStage = 1;

	int32 _curStarCount = 0;
	int32 _maxStarCount = 0;

	UIPanel* _completePanel = nullptr;
};