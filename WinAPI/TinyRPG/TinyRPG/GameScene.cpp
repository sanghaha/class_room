#include "pch.h"
#include "GameScene.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "TimeManager.h"
#include "UIManager.h"
#include "Map.h"
#include "Player.h"
#include "Effect.h"
#include "Enemy.h"
#include "Game.h"
#include "DataManager.h"
#include "MapData.h"
#include "MonsterData.h"
#include "Item.h"
#include "Projectile.h"

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
		Vector pos = _player->GetPos();
		Size mapSize = _map->GetMapSize();

		float halfSizeX = GWinSizeX / 2;
		float halfSizeY = GWinSizeY / 2;

		pos.x = ::clamp(pos.x, halfSizeX, mapSize.Width - halfSizeX);
		pos.y = ::clamp(pos.y, halfSizeY, mapSize.Height - halfSizeY);

		SetCameraPos(pos);
	}
}

void GameScene::Render(ID2D1HwndRenderTarget* renderTarget)
{
	Super::Render(renderTarget);

	UIManager::GetInstance()->Render(renderTarget);
}

void GameScene::loadResources()
{
	//ResourceManager::GetInstance()->LoadTexture(L"TestMap", L"TestMap.bmp");
	ResourceManager::GetInstance()->LoadSprite(L"TileMap", L"Tilemap_Elevation.png", 16, 8);	
	ResourceManager::GetInstance()->LoadSprite(L"Warrior_Blue", L"Player/Warrior_Blue.png", 6, 8);
	ResourceManager::GetInstance()->LoadSprite(L"Bow_Blue", L"Player/Archer_Blue.png", 8, 7);
	ResourceManager::GetInstance()->LoadSprite(L"Torch_Red", L"Monster/Torch_Red.png", 7, 5);
	ResourceManager::GetInstance()->LoadSprite(L"Explosion", L"Effect/Explosions.png", 9, 1);
	ResourceManager::GetInstance()->LoadSprite(L"Items", L"Item/Items.png", 16, 22);
	ResourceManager::GetInstance()->LoadSprite(L"HudItems", L"Item/Items.png", 16, 22);
	ResourceManager::GetInstance()->LoadSprite(L"HudIcons", L"UI/HudIcons.png", 2, 1);
	ResourceManager::GetInstance()->LoadSprite(L"Arrow", L"Player/Arrow.png", 1, 2);
	ResourceManager::GetInstance()->LoadPNGTexture(L"InventoryBG", L"UI/Inventory_Example_03.png");
	ResourceManager::GetInstance()->LoadPNGTexture(L"EquipBG", L"UI/Inventory_Example_02.png");
	ResourceManager::GetInstance()->LoadPNGTexture(L"InventorySelected", L"UI/Inventory_Slot_1.png", 48, 48);
	ResourceManager::GetInstance()->LoadPNGTexture(L"MapSelector", L"Map_Selector.png");
}

void GameScene::createObjects()
{
	const MapData* mapData = DataManager::GetInstance()->GetData<MapData>(L"MapData");
	if (nullptr == mapData)
		return;

	{
		fs::path path = ResourceManager::GetInstance()->GetResourcePath() / mapData->_tileMapPath;
		Sprite* sprite = ResourceManager::GetInstance()->GetSprite(L"TileMap");
		Map* map = new Map(Vector{ 0, 0 });
		map->SetSprite(sprite);
		map->LoadTileMap(path.c_str());
		addActor(map);

		_map = map;

		// �� ������ �о�ͼ� Cell ���� ����
		CreateGrid();
	}
	{
		Sprite* sprite = ResourceManager::GetInstance()->GetSprite(L"Warrior_Blue");
		Player* player = new Player(Vector{ 160, 160 });
		player->SetTexture(sprite);
		addActor(player);
	}
	{
		deque<Cell> spawnCell =_canMoveCell;

		const MonsterData* monsterData = DataManager::GetInstance()->GetMonsterData(1000);

		Vector initPos(320, 320);
		// ������ ��ǥ�� ���� ����
		for (int32 i = 0; i < mapData->_monsterCount; ++i)
		{
			int32 randIndex = rand() % spawnCell.size();
			Cell randomCell = spawnCell[randIndex];

			//Vector pos = initPos;
			//pos.x += (i * GTileSize);
			Vector pos(randomCell.ConvertToPos());
			Enemy* enmey = new Enemy(monsterData, pos);
			Sprite* sprite = ResourceManager::GetInstance()->GetSprite(L"Torch_Red");
			enmey->SetTexture(sprite);
			addActor(enmey);

			spawnCell.erase(spawnCell.begin() + randIndex);
		}
	}
}

void GameScene::CreateGrid()
{
	if (_map == nullptr)
		return;

	const MapData* data = DataManager::GetInstance()->GetData<MapData>(L"MapData");
	if (nullptr == data)
		return;

	auto lamdaCanMoveTile = [](const MapData* data, int32 tileX, int32 tileY)
		{
			for (auto iter : data->_canMoveTile)
			{
				if (tileX >= iter.minX && tileX <= iter.maxX && tileY >= iter.minY && tileY <= iter.maxY)
					return true;
			}
			return false;
		};

	// �׸��� ����
	_gridCountX = _map->GetGridWidth();
	_gridCountY = _map->GetGridHeight();

	for (int32 i = 0; i < _gridCountX; ++i)
	{
		for (int32 j = 0; j < _gridCountY; ++j)
		{
			Cell cell{ i, j };
			GridInfo gridInfo;

			int32 tileX = -1, tileY = -1;
			_map->ConvertTopTileIndex(i, j, tileX, tileY);
			gridInfo.canMoveCell = lamdaCanMoveTile(data, tileX, tileY);

			_grid.emplace(cell, gridInfo);

			if (gridInfo.canMoveCell)
			{
				_canMoveCell.emplace_back(cell);
			}
		}
	}
}

void GameScene::initTimer()
{
}

bool GameScene::CanMove(Cell cell)
{
	auto find = _grid.find(cell);
	if (find != _grid.end())
	{
		// ������ �ִٸ� ����
		//if (find->second._actors.size() != 0)
		if(find->second.blockedCount != 0)
			return false;

		return find->second.canMoveCell;
	}
	return false;
}

bool GameScene::FindPath(Cell start, Cell end, deque<Cell>& findPath, int32 maxDepth)
{
	findPath.clear();

	// �ʹ� �ָ� ����
	int32 depth = start.DeltaLength(end);
	if (depth >= maxDepth)
		return false;

	// ū������ �Ųٷ� �̾ư����ϴϱ�, ������ �־ �ǰ�, stl�� grater�� �־ �ȴ�.
	priority_queue<PQNode, vector<PQNode>, greater<PQNode>> pq;
	map<Cell, int32> best;	
	map<Cell, Cell> parent;

	// ������ ���
	// �湮�ߴ� �θ��� ������ �߰�

	// �ʱⰪ ����
	{
		// ���� �Ÿ��� ���� ����. �̰��� �� ����� �ȴ�.
		int32 cost = start.DeltaLength(end);

		// ������������ ����
		pq.push(PQNode(cost, start));
		best[start] = cost;
		parent[start] = start;
	}


	bool found = false;
	// �ʿ��� ��带 ��� ��ȸ�ߴ��� Ȯ��
	while (pq.empty() == false)
	{
		// ������ ��带 �湮�ؼ�, ���� ���� �ĺ��� ã�´�.
		PQNode node = pq.top();
		pq.pop();

		// �̹� ���� ��θ� ã�Ҵ�.
		if (best[node.pos] < node.cost)
		{
			continue;
		}

		// �������� ���������� ����
		if (node.pos == end)
		{
			found = true;
			break;
		}

		// �����¿�. ������ ��带 �湮�ؼ� �� ���� ����� ��尡 �ִ��� Ȯ���Ѵ�.
		for (int32 dir = 0; dir < DirType::DIR_MAX; ++dir)
		{
			Cell nextCell = node.pos.NextCell((DirType)dir);

			// ������ ���� ���� ���� �����̸� ����
			if (CanMove(nextCell) == false)
			{
				continue;
			}

			// �湮�ؾ��ϴ� ���� �������� �ʹ� �ָ� ����
			int32 depth = nextCell.DeltaLength(start);
			if (depth >= maxDepth)
				continue;

			int32 cost = nextCell.DeltaLength(end);

			// ó�� �湮�ϴ� ��尡 �ƴ϶��, ��� ��
			if (best.find(nextCell) != best.end())
			{
				int32 bestCost = best[nextCell];
				if (cost != 0)
				{
					// �ٸ� ��ο��� �� �������� ã������ ��ŵ�Ѵ�.
					if (bestCost <= cost)
					{
						continue;
					}
				}
			}

			// ������ ����
			best[nextCell] = cost;
			pq.push(PQNode(cost, nextCell));
			parent[nextCell] = node.pos;
		}
	}

	// ���������� ���� �����ִ�.
	if (found == false)
	{
		// ���������� ���� �������� �Ѱ��ش�.
		int32 bestCost = INT_MAX;
		for (auto& iter : best)
		{
			Cell pos = iter.first;
			int32 cost = iter.second;

			// ���� cost�� best cost�� ���ٸ�, �׳��� ���������� ����� �Ÿ��� �ִ� �༮�� ����ش�.
			if (bestCost == cost)
			{
				int32 distEnd = end.DeltaLength(start);
				int32 distPos = pos.DeltaLength(start);
				if (distPos < distEnd)
				{
					// �������� ���� ��ġ�� �������ش�.
					end = pos;
				}
			}
			else if (cost < bestCost)
			{
				end = pos;
				bestCost = cost;
			}
		}
	}

	findPath.clear();
	Cell pos = end;
	
	// �湮�ߴ� ����Ʈ�� ã�ư��鼭 ���� path�� �־��ش�.
	while (true)
	{
		if (pos == start)
			break;

		findPath.push_back(pos);

		// ������
		if (pos == parent[pos])
			break;

		pos = parent[pos];
	}

	std::reverse(findPath.begin(), findPath.end());
	return false;
}

void GameScene::CreateExplosionEffect(Vector pos)
{
	EffectExplosion* effect = new EffectExplosion(pos);

	// ���� �ý��ۿ� �ִ´�.
	_reserveAdd.emplace(effect);
}

void GameScene::CreateDropItem(Vector pos, int32 itemId)
{
	const ItemData* itemData = DataManager::GetInstance()->GetItemData(itemId);
	if (nullptr == itemData)
		return;

	DropItem* item = new DropItem(pos, itemData);

	// ���� �ý��ۿ� �ִ´�.
	_reserveAdd.emplace(item);
}

void GameScene::CreateArrow(Vector pos, DirType dir, Cell dest, int32 attak)
{
	Projectile* arrow = new Projectile(pos);
	arrow->SetProjectileInfo(dir, dest, attak);

	// ���� �ý��ۿ� �ִ´�.
	_reserveAdd.emplace(arrow);
}
