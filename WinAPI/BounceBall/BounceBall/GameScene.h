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

protected:
	void loadStage(std::wstring fileName);

private:
	UIImage* _bg = nullptr;

	class Ball* _ball = nullptr;
};

