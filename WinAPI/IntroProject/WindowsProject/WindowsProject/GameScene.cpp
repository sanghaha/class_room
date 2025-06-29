#include "pch.h"
#include "GameScene.h"
#include "Player.h"
#include "Enemy.h"
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
}

void GameScene::Render(HDC hdc)
{
	_player->Render(hdc);
	_enemy->Render(hdc);
}
