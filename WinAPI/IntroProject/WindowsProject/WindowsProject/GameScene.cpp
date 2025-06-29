#include "pch.h"
#include "GameScene.h"
#include "Player.h"
#include "Enemy.h"
#include "Missile.h"


void GameScene::Init()
{
	_player = new Player();
	_player->Init();

	_enemy = new Enemy();
	_enemy->Init();
}

void GameScene::Update(float deltaTime)
{
	_player->Update(deltaTime);
	_enemy->Update(deltaTime);

	for (auto iter : _missile)
	{
		iter->Update(deltaTime);
	}
}

void GameScene::Render(HDC hdc)
{
	_player->Render(hdc);
	_enemy->Render(hdc);

	for (auto iter : _missile)
	{
		iter->Render(hdc);
	}
}

void GameScene::CreateMissile(Vector pos, float angle, class Enemy* target)
{
	Missile* missile = new Missile();
	missile->Init(pos, angle, target);
	_missile.push_back(missile);
}

void GameScene::RemoveMissile(Missile* missile)
{
	auto it = std::find(_missile.begin(), _missile.end(), missile);
	_missile.erase(it);

	delete missile;
}
