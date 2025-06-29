#pragma once

#include "Scene.h"

class GameScene : public Scene
{
public:
	virtual void Init();
	virtual void Update(float deltaTime);
	virtual void Render(HDC hdc);

	void CreateMissile(Vector pos, float angle);
	void RemoveMissile(class Missile* missile);

private:
	class Player* _player = nullptr;
	class Enemy* _enemy = nullptr;
	vector<class Missile*>_missile;
};

