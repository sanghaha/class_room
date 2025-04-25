#include "pch.h"
#include "GameScene.h"
#include "BitmapTexture.h"
#include "ResourceManager.h"
#include "TimeManager.h"
#include "UIManager.h"
#include "Map.h"
#include "Player.h"
#include "Effect.h"
#include "Enemy.h"
#include "Game.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::Init()
{
	Super::Init();

	// UI �Ŵ��� �ʱ�ȭ
	UIManager::GetInstance()->Init();
}

void GameScene::Update(float deltaTime)
{
	Super::Update(deltaTime);

	// ĳ���� ��ġ �������� ī�޶� ��ǥ�� �׻� �������ش�.
	if (_player && _map)
	{
		Pos pos = _player->GetPos();
		Size mapSize = _map->GetMapSize();

		float halfSizeX = GWinSizeX / 2;
		float halfSizeY = GWinSizeY / 2;

		pos.x = ::clamp(pos.x, halfSizeX, mapSize.Width - halfSizeX);
		pos.y = ::clamp(pos.y, halfSizeY, mapSize.Height - halfSizeY);

		SetCameraPos(pos);
	}
}

void GameScene::Render(HDC hdc)
{
	Super::Render(hdc);

	UIManager::GetInstance()->Render(hdc);
}

void GameScene::loadResources()
{
	//ResourceManager::GetInstance()->LoadTexture(L"TestMap", L"TestMap.bmp");
	ResourceManager::GetInstance()->LoadSprite(L"TileMap", L"Tilemap_Elevation.bmp", RGB(255,0,255), 16, 8);
	//ResourceManager::GetInstance()->LoadSprite(L"Warrior_Blue", L"Player/Warrior_Blue.bmp", RGB(255, 0, 255), 6, 8);
	ResourceManager::GetInstance()->LoadPNGSprite(L"Warrior_Blue", L"Player/Warrior_Blue.png", 6, 8);
}

void GameScene::createObjects()
{
	{
		fs::path path = ResourceManager::GetInstance()->GetResourcePath() / L"State1.tilemap";
		Sprite* sprite = ResourceManager::GetInstance()->GetSprite(L"TileMap");
		Map* map = new Map(Pos{ 0, 0 });
		map->SetSprite(sprite);
		map->LoadTileMap(path.c_str());
		addActor(map);

		_map = map;
	}
	{
		Sprite* sprite = ResourceManager::GetInstance()->GetSprite(L"Warrior_Blue");
		Player* player = new Player(Pos{ (float)(GWinSizeX / 2), (float)(GWinSizeY / 2) });
		player->SetTexture(sprite);
		addActor(player);

		_player = player;
	}
	{
		
	}
}

void GameScene::initTimer()
{
}
