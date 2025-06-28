#include "pch.h"
#include "GameScene.h"
#include "Player.h"

void GameScene::Init()
{
	_player = new Player();
	_player->Init();
}

void GameScene::Update(float deltaTime)
{
	_player->Update(deltaTime);
}

void GameScene::Render(HDC hdc)
{
	_player->Render(hdc);
}
