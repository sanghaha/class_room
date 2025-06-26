#include "pch.h"
#include "GameScene.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "TimeManager.h"
#include "UIManager.h"
#include "Map.h"
#include "FixedMap.h"
#include "Player.h"
#include "Bullet.h"
#include "Effect.h"
#include "Enemy.h"
#include "Game.h"
#include "InputManager.h"
#include "ObjectPool.h"
#include "DataManager.h"
#include "ResourceData.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::Init()
{
	Super::Init();

	_enemyBulletPool.Init<Bullet>(4, [this]()
		{
			return createEnemyBulletObjectPool();
		}
	);

	_playerBulletPool.Init<Bullet>(4, [this]()
		{
			return createPlayerBulletObjectPool();
		}
	);
}

void GameScene::Update(float deltaTime)
{
	Super::Update(deltaTime);

	// 캐릭터 위치 기준으로 카메라 좌표값 항상 갱신해준다.
	if (_player && _map)
	{
		Pos pos = _player->GetPos();
		Size mapSize = _map->GetMapSize();

		float halfSizeX = GWinSizeX / 2;
		float halfSizeY = GWinSizeY / 2;

		pos.x = ::clamp(pos.x, halfSizeX, mapSize.w - halfSizeX);
		pos.y = ::clamp(pos.y, halfSizeY, mapSize.h - halfSizeY);

		SetCameraPos(pos);
	}

	if (InputManager::GetInstance()->GetButtonDown(KeyType::F1))
	{
		ColliderCircle::drawDebug = !ColliderCircle::drawDebug;
	}

	if (InputManager::GetInstance()->GetButtonDown(KeyType::F2))
	{
		Scene::drawDebugCell = !Scene::drawDebugCell;
	}
}

void GameScene::Render(HDC hdc)
{
	Super::Render(hdc);

	UIManager::GetInstance()->Render(hdc);
}

void GameScene::CreatePlayerBullet(Pos pos)
{
	//Bullet* bullet = new Bullet(pos, L"PlayerBullet", 0, BulletType::BT_Player);
	Bullet* bullet = _playerBulletPool.Acquire<Bullet>([this]()
		{
			return createPlayerBulletObjectPool();
		});
	if (nullptr == bullet)
		return;

	bullet->Reset(pos, 0);
	bullet->SetDir(Dir{ 0 ,-1 }); // 위쪽으로 발사

	// 예약 시스템에 넣는다.
	_reserveAdd.emplace(bullet);
}

void GameScene::CreateEnemyBullet(Pos pos, int32 bulletIndex)
{	
	//Bullet* bullet = new Bullet(pos, L"EnemyBullet", bulletIndex, BulletType::BT_Enemy);
	Bullet* bullet = _enemyBulletPool.Acquire<Bullet>([this]()
		{
			return createEnemyBulletObjectPool();
		}
	);
	if (nullptr == bullet)
		return;

	bullet->Reset(pos, bulletIndex);
	bullet->SetDir(Dir{ 0 , 1 }); // 아래쪽으로 발사

	// 예약 시스템에 넣는다.
	_reserveAdd.emplace(bullet);
}

void GameScene::CreateExplosion(Pos pos)
{
	Effect* effect = new Effect(pos, L"Explosion", 0.7f);
	if (effect == nullptr)
		return;

	// 예약 시스템에 넣는다.
	_reserveAdd.emplace(effect);
}

void GameScene::CreateRandomEnemy()
{
	const HBitmapInfo* info = ResourceManager::GetInstance()->GetHBitmap(L"EnemyBullet");
	if (nullptr == info)
		return;

	int bulletIndex = rand() % info->countX;

	wstring textureKey[4] = { L"Enemy1", L"Enemy2", L"Enemy3", L"Enemy4" };
	int randomIndex = rand() % 4;

	const int32 enemyCount = 4;
	Pos pos{ 50, 100 };
	int32 xDelta = GetMapSize().w / enemyCount;

	for (int32 i = 0; i < enemyCount; ++i)
	{
		Enemy* enemy = new Enemy(Pos{ pos.x + (xDelta * i), pos.y }, textureKey[randomIndex], bulletIndex);
		addActor(enemy);
	}
}


Size GameScene::GetMapSize() const
{
	if (_map)
	{
		return _map->GetMapSize();
	}
	return Super::GetMapSize();
}

void GameScene::loadResources()
{
	const ResourceData* resourceData = DataManager::GetInstance()->GetData<ResourceData>(L"ResourceData");
	if (!resourceData)
		return;

	for (auto iter : resourceData->GetGameSceneResource())
	{
		const auto item = iter.second;
		ResourceManager::GetInstance()->LoadHBitmap(
			item->key, 
			item->fileName, 
			item->transparent,
			item->countX,
			item->countY,
			item->loop);
	}

	/*
	ResourceManager::GetInstance()->LoadHBitmap(L"BG", L"BG.bmp", -1);
	ResourceManager::GetInstance()->LoadHBitmap(L"Player", L"Player.bmp", RGB(252, 0, 255));
	ResourceManager::GetInstance()->LoadHBitmap(L"Enemy1", L"Enemy1.bmp", RGB(255, 0, 255));
	ResourceManager::GetInstance()->LoadHBitmap(L"Enemy2", L"Enemy2.bmp", RGB(255, 0, 255));
	ResourceManager::GetInstance()->LoadHBitmap(L"Enemy3", L"Enemy3.bmp", RGB(255, 0, 255));
	ResourceManager::GetInstance()->LoadHBitmap(L"Enemy4", L"Enemy4.bmp", RGB(255, 0, 255));
	ResourceManager::GetInstance()->LoadHBitmap(L"PlayerBullet", L"PlayerBullet.bmp", RGB(252, 0, 255));
	ResourceManager::GetInstance()->LoadHBitmap(L"PlayerHP", L"PlayerHP.bmp", RGB(252, 0, 255));
	ResourceManager::GetInstance()->LoadHBitmap(L"EnemyBullet", L"EnemyBullet.bmp", -1, 5, 1, true);
	ResourceManager::GetInstance()->LoadHBitmap(L"Explosion", L"explosion.bmp", RGB(0, 0, 0), 6, 2, false);
	*/
}

void GameScene::createObjects()
{
	{
		Map* map = new Map(Pos{ 0, 0 });
		//FixedMap* map = new FixedMap(Pos{ 0, 0 });
		//_map = map;
		addActor(map);
	}
	{
		Pos initPos{ (float)(GetMapSize().w / 2), (float)(GetMapSize().h - 200) };
		Player* player = new Player(initPos, L"Player");
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

void GameScene::addActor(Actor* actor)
{
	if (RenderLayer::RL_Player == actor->GetRenderLayer())
	{
		_player = dynamic_cast<Player*>(actor);
	}

	Super::addActor(actor);
}

void GameScene::removeActor(Actor* actor)
{
	if (RenderLayer::RL_Player == actor->GetRenderLayer())
	{
		_player = nullptr;
	}

	Super::removeActor(actor);
}

Bullet* GameScene::createEnemyBulletObjectPool()
{
	Bullet* bullet = new Bullet(Pos(0,0), L"EnemyBullet", 0, BulletType::BT_Enemy);
	bullet->SetObjectPool(&_enemyBulletPool);
	return bullet;
}

Bullet* GameScene::createPlayerBulletObjectPool()
{
	Bullet* bullet = new Bullet(Pos(0, 0), L"PlayerBullet", 0, BulletType::BT_Player);
	bullet->SetObjectPool(&_playerBulletPool);
	return bullet;
}
