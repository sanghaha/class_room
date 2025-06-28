#pragma once

#include "Scene.h"

class GameScene : public Scene
{
public:
	virtual void Init();
	virtual void Update(float deltaTime);
	virtual void Render(HDC hdc);

private:
	class Player* _player = nullptr;
};

