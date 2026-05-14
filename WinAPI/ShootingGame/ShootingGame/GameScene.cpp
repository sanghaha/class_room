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

	_enemyBulletPool.Init<Bullet>(4);
	_playerBulletPool.Init<Bullet>(4);
}

void GameScene::Update(float deltaTime)
{
	Super::Update(deltaTime);

	// 캐릭터 위치 기준으로 카메라 좌표값 항상 갱신해준다.
	if (_player && _map)
	{
		Pos pos = _player->GetPos();
		//Pos pos = _cameraPos;
		Size mapSize = _map->GetMapSize();

		float halfSizeX = GWinSizeX / 2;
		float halfSizeY = GWinSizeY / 2;

		//pos.y -= 100 * deltaTime;

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
	Bullet* bullet = _playerBulletPool.Acquire<Bullet>();
	if (nullptr == bullet)
		return;

	bullet->Init(pos, L"PlayerBullet", 0, BulletType::BT_Player);
	bullet->SetDir(Dir{ 0 ,-1 }); // 위쪽으로 발사

	// 예약 시스템에 넣는다.
	_reserveAdd.emplace(bullet);
}

void GameScene::CreateEnemyBullet(Pos pos, int32 bulletIndex)
{	
	Bullet* bullet = _enemyBulletPool.Acquire<Bullet>();
	if (nullptr == bullet)
		return;

	bullet->Init(pos, L"EnemyBullet", bulletIndex, BulletType::BT_Enemy);
	bullet->SetDir(Dir{ 0 , 1 }); // 아래쪽으로 발사

	// 예약 시스템에 넣는다.
	_reserveAdd.emplace(bullet);
}

void GameScene::CreateExplosion(Pos pos)
{
	Effect* effect = new Effect();
	if (effect == nullptr)
		return;

	effect->Init(pos, L"Explosion", 0.7f);

	// 예약 시스템에 넣는다.
	_reserveAdd.emplace(effect);
}

void GameScene::CreateRandomEnemy()
{
	int bulletIndex = rand() % 5;

	wstring textureKey[4] = { L"Enemy1", L"Enemy2", L"Enemy3", L"Enemy4" };
	int randomIndex = rand() % 4;

	const int32 enemyCount = 4;
	Pos pos{ 50, 100 };
	int32 xDelta = GetMapSize().w / enemyCount;

	for (int32 i = 0; i < enemyCount; ++i)
	{
		Enemy* enemy = new Enemy();
		enemy->Init(Pos{ pos.x + (xDelta * i), pos.y }, textureKey[randomIndex], bulletIndex);
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
		ResourceManager::GetInstance()->LoadTexture(
			item->key,
			item->fileName,
			item->countX,
			item->countY,
			item->transparent,
			item->dur);
	}
}

void GameScene::createObjects()
{
	{
		_scrollingMap = new Map();
		_scrollingMap->Init(Pos{ 0, 0 });
		addActor(_scrollingMap);

		//_map = new FixedMap();
		//_map->Init(Pos{ 0, 0 });
		//addActor(_map);
	}
	{
		Pos initPos{ (float)(GetMapSize().w / 2), (float)(GetMapSize().h - 200) };
		Player* player = new Player();
		player->Init(initPos, L"Player");
		addActor(player);

		SetCameraPos(initPos);
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

		TimeManager::GetInstance()->AddTimer(timer);
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
