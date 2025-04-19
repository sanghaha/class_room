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
	// ���ҽ� �ε�
	loadResources();

	// ������Ʈ ����
	createObjects();

	// Ÿ�̸� �߰�
	{
		Timer timer(
			[]()
			{
				Scene::GetInstance()->CreateRandomEnemy();
			},
			2.0f);

		TimeManager::GetInstance()->AddTimer(std::move(timer));
	}

	// �׸��� ����
	_gridCountX = GWinSizeX / _gridSize;
	_gridCountY = GWinSizeY / _gridSize;
	for (int32 i = 0; i < _gridCountX; ++i)
	{
		for (int32 j = 0; j < _gridCountY; ++j)
		{
			Cell cell{ i, j };
			GridInfo gridInfo;
			_grid.emplace(std::move(cell), std::move(gridInfo));
		}
	}
}

void Scene::Update(float deltaTime)
{
	for (auto actor : _actors)
	{
		actor->Update(deltaTime);
	}

	// �߰��� ���� ó��
	for (auto actor : _reserveAdd)
	{
		// ����� ���͸� ������ ����Ʈ�� �߰��Ѵ�.
		addActor(actor);
	}
	_reserveAdd.clear(); // �߰� �� ���� ����Ʈ �ʱ�ȭ

	// ������ ���� ó��
	for (Actor* actor : _reserveRemove)
	{
		removeActor(actor);
	}

	_reserveRemove.clear(); // ���� �� ���� ����Ʈ �ʱ�ȭ
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
	
	// �׸��� ����׿�
	drawGrid(hdc);
}

void Scene::drawGrid(HDC hdc)
{
	// ȭ�� ũ��� �׸��� ũ�� ����
	int32 width = GWinSizeX;
	int32 height = GWinSizeY;

	// ������ �� ����
	HPEN redPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	HPEN oldPen = (HPEN)SelectObject(hdc, redPen);

	// ���μ� �׸���
	for (int y = 0; y <= height; y += _gridSize)
	{
		MoveToEx(hdc, 0, y, nullptr); // ������ ����
		LineTo(hdc, width, y);        // �������� �� �׸���
	}

	// ���μ� �׸���
	for (int x = 0; x <= width; x += _gridSize)
	{
		MoveToEx(hdc, x, 0, nullptr); // ������ ����
		LineTo(hdc, x, height);       // �������� �� �׸���
	}

	// ���� �� ���� �� �� �� ����
	SelectObject(hdc, oldPen);
	DeleteObject(redPen);
}

void Scene::CreatePlayerBullet(Pos pos)
{
	Sprite* sprite = ResourceManager::GetInstance()->GetSprite(L"PlayerBullet");
	Bullet* bullet = new Bullet(pos);
	bullet->SetResource(sprite);
	bullet->SetDir(Dir{ 0 ,-1 }); // �������� �߻�
	bullet->SetBulletType(BulletType::BT_Player); // �÷��̾� �Ѿ˷� ����

	// ���� �ý��ۿ� �ִ´�.
	_reserveAdd.push_back(bullet);
}

void Scene::CreateEnemyBullet(Pos pos)
{
	Sprite* sprite = ResourceManager::GetInstance()->GetSprite(L"EnemyBullet");
	Bullet* bullet = new Bullet(pos);
	bullet->SetResource(sprite);
	bullet->SetDir(Dir{ 0 , 1 }); // �Ʒ������� �߻�
	bullet->SetBulletType(BulletType::BT_Enemy); // �� �Ѿ˷� ����

	// ���� �ý��ۿ� �ִ´�.
	_reserveAdd.push_back(bullet);
}

void Scene::CreateExplosion(Pos pos)
{
	Sprite* sprite = ResourceManager::GetInstance()->GetSprite(L"Explosion");

	Effect* effect = new Effect(pos);
	effect->SetTexture(sprite, 0.05f);

	// ���� �ý��ۿ� �ִ´�.
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

void Scene::UpdateGrid(Actor* actor, Pos prevPos, Pos nextPos)
{
	// ������ ��ġ�� ����Ǿ����� �׸��� ����
	Cell prevCell = Cell::ConvertToCell(prevPos, _gridSize);
	Cell currCell = Cell::ConvertToCell(nextPos, _gridSize);

	// ������ ���� �ʿ� ����
	if (prevCell == currCell)
		return;

	// ���� ������ ����
	{
		auto find = _grid.find(prevCell);
		if (find != _grid.end())
		{
			auto& gridInfo = find->second;
			auto iter = gridInfo._actors.find(actor);
			if (iter != gridInfo._actors.end())
			{
				gridInfo._actors.erase(iter);
			}
		}
	}

	// ���� �߰��Ϸ��� ���� ���� ���̸� ����
	if (currCell.index_X < 0 || currCell.index_X >= _gridCountX || currCell.index_Y < 0 || currCell.index_Y >= _gridCountY)
		return;

	// ���� ���� �߰�
	{
		auto find = _grid.find(currCell);
		if (find != _grid.end())
		{
			auto& gridInfo = find->second;
			if (false == gridInfo._actors.contains(actor))
			{
				gridInfo._actors.emplace(actor);
			}
		}
	}
}

const GridInfo& Scene::GetGridInfo(const Cell& cell)
{
	auto find = _grid.find(cell);
	if (find != _grid.end())
	{
		return find->second;
	}

	static GridInfo emptyGridInfo;
	return emptyGridInfo;
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

	UpdateGrid(actor, actor->GetPos(), Pos{-1,-1});

	// ���� ����Ʈ���� ����
	{
		auto& list = _renderList[actor->GetRenderLayer()];
		auto iter = std::find(list.begin(), list.end(), actor);
		if (iter != list.end())
		{
			list.erase(iter);
		}
	}

	// ���� actor set ���� ����
	{
		auto iter = _actors.find(actor);
		if (iter != _actors.end())
		{
			_actors.erase(iter);
			delete actor;
		}
	}
}
