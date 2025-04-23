#include "pch.h"
#include "GameScene.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "TimeManager.h"
#include "UIManager.h"
#include "Map.h"
#include "Player.h"
#include "Bullet.h"
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

	// UI 매니저 초기화
	UIManager::GetInstance()->Init();
}

void GameScene::Render(HDC hdc)
{
	Super::Render(hdc);

	UIManager::GetInstance()->Render(hdc);
}

void GameScene::CreatePlayerBullet(Pos pos)
{
	Sprite* sprite = ResourceManager::GetInstance()->GetSprite(L"PlayerBullet");
	Bullet* bullet = new Bullet(pos);
	bullet->SetResource(sprite);
	bullet->SetDir(Dir{ 0 ,-1 }); // 위쪽으로 발사
	bullet->SetBulletType(BulletType::BT_Player); // 플레이어 총알로 설정

	// 예약 시스템에 넣는다.
	_reserveAdd.emplace(bullet);
}

void GameScene::CreateEnemyBullet(Pos pos)
{
	Sprite* sprite = ResourceManager::GetInstance()->GetSprite(L"EnemyBullet");
	Bullet* bullet = new Bullet(pos);
	bullet->SetResource(sprite);
	bullet->SetDir(Dir{ 0 , 1 }); // 아래쪽으로 발사
	bullet->SetBulletType(BulletType::BT_Enemy); // 적 총알로 설정

	// 예약 시스템에 넣는다.
	_reserveAdd.emplace(bullet);
}

void GameScene::CreateExplosion(Pos pos)
{
	Sprite* sprite = ResourceManager::GetInstance()->GetSprite(L"Explosion");

	Effect* effect = new Effect(pos);
	effect->SetTexture(sprite, 0.05f);

	// 예약 시스템에 넣는다.
	_reserveAdd.emplace(effect);
}

void GameScene::CreateRandomEnemy()
{
	wstring textureKey[4] = { L"Enemy1", L"Enemy2", L"Enemy3", L"Enemy4" };
	int randomIndex = rand() % 4;
	Texture* texture = ResourceManager::GetInstance()->GetTexture(textureKey[randomIndex]);

	const int32 enemyCount = 4;
	Pos pos{ 50, 100 };
	int32 xDelta = GWinSizeX / enemyCount;

	for (int32 i = 0; i < enemyCount; ++i)
	{
		Enemy* enemy = new Enemy(Pos{ pos.x + (xDelta * i), pos.y });
		enemy->SetTexture(texture);
		addActor(enemy);
	}
}


void GameScene::loadResources()
{
	ResourceManager::GetInstance()->LoadTexture(L"BG", L"BG.bmp");
	ResourceManager::GetInstance()->LoadTexture(L"Player", L"Player.bmp", RGB(252, 0, 255));
	ResourceManager::GetInstance()->LoadTexture(L"Enemy1", L"Enemy1.bmp", RGB(255, 0, 255));
	ResourceManager::GetInstance()->LoadTexture(L"Enemy2", L"Enemy2.bmp", RGB(255, 0, 255));
	ResourceManager::GetInstance()->LoadTexture(L"Enemy3", L"Enemy3.bmp", RGB(255, 0, 255));
	ResourceManager::GetInstance()->LoadTexture(L"Enemy4", L"Enemy4.bmp", RGB(255, 0, 255));
	ResourceManager::GetInstance()->LoadTexture(L"PlayerBullet", L"PlayerBullet.bmp", RGB(252, 0, 255));
	ResourceManager::GetInstance()->LoadTexture(L"PlayerHP", L"PlayerHP.bmp", RGB(252, 0, 255));


	ResourceManager::GetInstance()->LoadSprite(L"EnemyBullet", L"EnemyBullet.bmp", 0, 5, 1, true);
	ResourceManager::GetInstance()->LoadSprite(L"Explosion", L"explosion.bmp", RGB(0, 0, 0), 6, 2, false);
	ResourceManager::GetInstance()->LoadSprite(L"PlayerBullet", L"PlayerBullet.bmp", 0, 1, 1, true);
}

void GameScene::createObjects()
{
	{
		Texture* texture = ResourceManager::GetInstance()->GetTexture(L"BG");
		Map* map = new Map(Pos{ 0, 0 });
		map->SetTexture(texture);
		addActor(map);
	}
	{
		Texture* texture = ResourceManager::GetInstance()->GetTexture(L"Player");
		Player* player = new Player(Pos{ (float)(GWinSizeX / 2), (float)(GWinSizeY - 200) });
		player->SetTexture(texture);
		addActor(player);
	}
	{
		CreateRandomEnemy();
	}
}

void GameScene::initTimer()
{
	{
		Timer timer(
			[]()
			{
				Game::GetGameScene()->CreateRandomEnemy();
			},
			2.0f);

		TimeManager::GetInstance()->AddTimer(std::move(timer));
	}
}
