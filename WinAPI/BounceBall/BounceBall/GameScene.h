#pragma once
#include "Scene.h"

class UIImage;

class GameScene : public Scene
{
	using Super = Scene;
public:
	GameScene();
	virtual ~GameScene();

	void Init() override;
	void Update(float deltaTime) override;
	void Render(ID2D1RenderTarget* renderTarget) override;

	bool CheckCollision(class Ball* ball, Vector start, Vector end, Vector& outNormal, float& depth);

	void CreateEffect(Vector pos, string spriteName);
	void AddStarCount();

protected:
	bool loadStage(int32 stage);
	void clearStage();

	void createCollisionMask();

	void onClickNextStageButton();

private:
	uint64 COLLISION_BIT_MASK_BLOCK = 0;	// Ball 액터와 충돌(block) 체크할 Bit mask
	uint64 COLLISION_BIT_MASK_OVERLAP = 0;	// Ball 액터와 충돌(overlap) 체크할 Bit mask

	int32 _currStage = 1;

	int32 _curStarCount = 0;
	int32 _maxStarCount = 0;

	UIImage* _completeImg = nullptr;
	UIButton* _nextStageButton = nullptr;
};

