#include "pch.h"
#include "Scene.h"
#include "ResourceManager.h"
#include "Map.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "ResourceBase.h"
#include "Texture.h"
#include "Sprite.h"
#include "Effect.h"
#include "TimeManager.h"

void Scene::Init()
{
	// 리소스 로드
	loadResources();

	// 오브젝트 생성
	createObjects();

	// 타이머 추가
	{
		Timer timer(
			[]()
			{
				Scene::GetInstance()->CreateRandomEnemy();
			},
			2.0f);

		TimeManager::GetInstance()->AddTimer(std::move(timer));
	}
}

void Scene::Update(float deltaTime)
{
	for (auto actor : _actors)
	{
		actor->Update(deltaTime);
	}

	// 추가된 예약 처리
	for (auto actor : _reserveAdd)
	{
		// 예약된 액터를 렌더링 리스트에 추가한다.
		addActor(actor);
	}
	_reserveAdd.clear(); // 추가 후 예약 리스트 초기화

	// 삭제된 예약 처리
	for (Actor* actor : _reserveRemove)
	{
		removeActor(actor);
	}

	_reserveRemove.clear(); // 삭제 후 예약 리스트 초기화
}

void Scene::Render(HDC hdc)
{
	for (auto list : _renderList)
	{
		for (auto actor : list)
		{
			actor->Render(hdc);
		}
	}
}

void Scene::CreatePlayerBullet(Pos pos)
{
	Sprite* sprite = ResourceManager::GetInstance()->GetSprite(L"PlayerBullet");
	Bullet* bullet = new Bullet(pos);
	bullet->SetResource(sprite);
	bullet->SetDir(Dir{ 0 ,-1 }); // 위쪽으로 발사
	bullet->SetBulletType(BulletType::BT_Player); // 플레이어 총알로 설정

	// 예약 시스템에 넣는다.
	_reserveAdd.push_back(bullet);
}

void Scene::CreateEnemyBullet(Pos pos)
{
	Sprite* sprite = ResourceManager::GetInstance()->GetSprite(L"EnemyBullet");
	Bullet* bullet = new Bullet(pos);
	bullet->SetResource(sprite);
	bullet->SetDir(Dir{ 0 , 1 }); // 아래쪽으로 발사
	bullet->SetBulletType(BulletType::BT_Enemy); // 적 총알로 설정

	// 예약 시스템에 넣는다.
	_reserveAdd.push_back(bullet);
}

void Scene::CreateExplosion(Pos pos)
{
	Sprite* sprite = ResourceManager::GetInstance()->GetSprite(L"Explosion");

	Effect* effect = new Effect(pos);
	effect->SetTexture(sprite, 0.05f);

	// 예약 시스템에 넣는다.
	_reserveAdd.push_back(effect);
}

void Scene::CreateRandomEnemy()
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

void Scene::ReserveRemove(Actor* actor)
{
	if (nullptr == actor)
		return;
	_reserveRemove.push_back(actor);
}

void Scene::loadResources()
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

void Scene::createObjects()
{
	{
		Texture* texture = ResourceManager::GetInstance()->GetTexture(L"BG");
		Map* map = new Map(Pos{0, 0});
		map->SetTexture(texture);
		addActor(map);
	}
	{
		Texture* texture = ResourceManager::GetInstance()->GetTexture(L"Player");
		Player* player = new Player(Pos{(float)(GWinSizeX / 2), (float)(GWinSizeY - 200)});
		player->SetTexture(texture);
		addActor(player);
	}
	{
		CreateRandomEnemy();
	}
}

void Scene::addActor(Actor* actor)
{
	if (nullptr == actor)
		return;

	if (actor->GetRenderLayer() < 0 || actor->GetRenderLayer() >= RenderLayer::RL_Count)
		return;

	actor->Init();
	_actors.emplace(actor);
	_renderList[actor->GetRenderLayer()].emplace_back(actor);
}

void Scene::removeActor(Actor* actor)
{
	if (nullptr == actor)
		return;

	if (actor->GetRenderLayer() < 0 || actor->GetRenderLayer() >= RenderLayer::RL_Count)
		return;

	// 렌더 리스트에서 제거
	{
		auto& list = _renderList[actor->GetRenderLayer()];
		auto iter = std::find(list.begin(), list.end(), actor);
		if (iter != list.end())
		{
			list.erase(iter);
		}
	}

	// 실제 actor set 에서 제거
	{
		auto iter = _actors.find(actor);
		if (iter != _actors.end())
		{
			_actors.erase(iter);
			delete actor;
		}
	}
}
