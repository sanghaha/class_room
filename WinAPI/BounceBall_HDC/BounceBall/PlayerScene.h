#pragma once
#include "Scene.h"

class PlayerScene : public Scene
{
	using Super = Scene;
public:
	PlayerScene();
	virtual ~PlayerScene();

	void Init() override;
	void Destory() override;
	void Update(float deltaTime) override;
	void Render(HDC renderTarget) override;

private:
	void loadStage();

	int32 _currStage = 1;
};
