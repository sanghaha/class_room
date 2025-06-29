#pragma once

#include "Scene.h"

class GameScene : public Scene
{
public:
	virtual void Init();
	virtual void Update(float deltaTime);
	virtual void Render(HDC hdc);

	void CreateMissile(Vector pos, float angle, class Enemy* target);
	void RemoveMissile(class Missile* missile);

	class Enemy* GetEnemy() { return _enemy; }

private:
	class Player* _player = nullptr;
	class Enemy* _enemy = nullptr;
	vector<class Missile*>_missile;
};

