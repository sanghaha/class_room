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
#include "Portal.h"
#include "InventorySystem.h"
#include "DXBitmap.h"
#include "UIMessage.h"
#include "UIHud.h"
#include "UIInventory.h"


GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::Init()
{
	Super::Init();

	

}

void GameScene::Update(float deltaTime)
{
	Super::Update(deltaTime);

	// 캐릭터 위치 기준으로 카메라 좌표값 항상 갱신해준다.
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

	_debugAStar.Update(deltaTime);
}

void GameScene::Render(ID2D1RenderTarget* renderTarget)
{
	Super::Render(renderTarget);

	_debugAStar.Render(renderTarget);
}

void GameScene::loadResources()
{
	//ResourceManager::GetInstance()->LoadTexture(L"TestMap", L"TestMap.bmp");
	ResourceManager::GetInstance()->LoadDXBitmap("TileMap", L"Tilemap_Elevation.png", 16, 8);
	ResourceManager::GetInstance()->LoadDXBitmap("Warrior_Blue", L"Player/Warrior_Blue.png", 6, 8);
	ResourceManager::GetInstance()->LoadDXBitmap("Bow_Blue", L"Player/Archer_Blue.png", 8, 7);
	ResourceManager::GetInstance()->LoadDXBitmap("Torch_Red", L"Monster/Torch_Red.png", 7, 5);
	ResourceManager::GetInstance()->LoadDXBitmap("Explosion", L"Effect/Explosions.png", 9, 1);
	ResourceManager::GetInstance()->LoadDXBitmap("Items", L"Item/Items.png", 16, 22);
	ResourceManager::GetInstance()->LoadDXBitmap("HudIcons", L"UI/HudIcons.png", 2, 1);
	ResourceManager::GetInstance()->LoadDXBitmap("Arrow", L"Player/Arrow.png", 1, 2);
	ResourceManager::GetInstance()->LoadDXBitmap("InventoryBG", L"UI/Inventory_Example_03.png", 1, 1);
	ResourceManager::GetInstance()->LoadDXBitmap("EquipBG", L"UI/Inventory_Example_02.png", 1, 1);
	ResourceManager::GetInstance()->LoadDXBitmap("InventorySelected", L"UI/Inventory_Slot_1.png", 48, 48);
	ResourceManager::GetInstance()->LoadDXBitmap("TileSelector", L"Tile_Selector.png", 1, 1);
	ResourceManager::GetInstance()->LoadDXBitmap("Numbers", L"UI/Numbers.png", 10, 1);
	ResourceManager::GetInstance()->LoadDXBitmap("Ribbon_Red_3Slides", L"UI/Ribbon_Red_3Slides.png", 1, 1);
	ResourceManager::GetInstance()->LoadDXBitmap("Health_03", L"UI/Health_03.png", 1, 1);
	ResourceManager::GetInstance()->LoadDXBitmap("Health_03_Bar01", L"UI/Health_03_Bar01.png", 1, 1);
}

void GameScene::createObjects()
{
	CreateStage(1);
}

void GameScene::createUI()
{
	{
		_uiHud = new UIHud();
		_ui.AddPanel(_uiHud);
	}
	{
		_uiInven = new UIInventory();
		_ui.AddPanel(_uiInven);
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

	// 그리드 생성
	_gridCountX = _map->GetGridWidth();
	_gridCountY = _map->GetGridHeight();

	for (int32 i = 0; i < _gridCountX; ++i)
	{
		for (int32 j = 0; j < _gridCountY; ++j)
		{
			Cell cell{ i, j };
			GridInfo gridInfo;

			int32 tileX = -1, tileY = -1;
			_map->ConvertGroundTileIndex(i, j, tileX, tileY);
			gridInfo.canMoveCell = lamdaCanMoveTile(data, tileX, tileY);

			_grid.emplace(cell, gridInfo);

			if (gridInfo.canMoveCell)
			{
				_canMoveCell.emplace_back(cell);
			}
		}
	}
}

bool GameScene::OnLeftClickEvent(int32 x, int32 y)
{
	if (Super::OnLeftClickEvent(x, y))
		return true;

	if (_player)
	{
		return _player->OnLeftClickEvent(x, y);
	}

	return false;
}

void GameScene::initTimer()
{
}

bool GameScene::CanMove(Cell cell)
{
	auto find = _grid.find(cell);
	if (find != _grid.end())
	{
		// 누군가 있다면 못감
		//if (find->second._actors.size() != 0)
		if(find->second.blockedCount != 0)
			return false;

		return find->second.canMoveCell;
	}
	return false;
}

// 맨해튼 거리 휴리스틱
int Heuristic(Cell curr, Cell end)
{
	// 목적지까지의 거리 비교
	return (abs(end.index_X - curr.index_X) + abs(end.index_Y - curr.index_Y)) * 10;
}

bool GameScene::FindPath(Cell start, Cell end, deque<Cell>& findPath, int32 maxDepth)
{
	findPath.clear();

	// 너무 멀면 무시
	int32 depth = start.DeltaLength(end);
	if (depth >= maxDepth)
		return false;

	if (start == end)
		return false;

	// 큰수부터 거꾸로 뽑아가야하니깐, 음수를 넣어도 되고, stl의 grater를 넣어도 된다.
	priority_queue<PQNode, vector<PQNode>, greater<PQNode>> pq;
	_debugAStar.best.clear();
	_debugAStar.parent.clear();
	_debugAStar.closedList.clear();
	_debugAStar.openList.clear();

	// 최적의 노드
	// 방문했던 부모의 개념을 추가

	// 초기값 설정
	{
		// 셀의 거리를 먼저 측정. 이것이 곧 비용이 된다.
		int g = 0;
		int h = Heuristic(start, end);
		int f = g + h;

		// 시작지점부터 측정
		pq.push(PQNode(g, h, start));
		_debugAStar.best[start] = f;
		_debugAStar.parent[start] = start;
		_debugAStar.openList[start] = PQNode(g, h, start);
	}

	int32 dirOrder[] = { DirType::DIR_RIGHT, DirType::DIR_DOWN, DirType::DIR_LEFT, DirType::DIR_UP };
	bool found = false;
	// 필요한 노드를 모두 순회했는지 확인
	while (pq.empty() == false)
	{
		// 인접한 노드를 방문해서, 제일 좋은 후보를 찾는다.
		PQNode node = pq.top();
		pq.pop();

		if (_debugAStar.closedList.count(node.pos) != 0)
		{
			continue;
		}

		// 이미 좋은 경로를 찾았다.
		if (_debugAStar.best[node.pos] < node.g + node.h)
		{
			continue;
		}

		// 목적지에 도착했으면 종료
		if (node.pos == end)
		{
			found = true;
			break;
		}

		_debugAStar.closedList.insert(node.pos);

		// 상하좌우. 인접한 노드를 방문해서 더 좋은 비용의 노드가 있는지 확인한다.
		for (int32 dir = 0; dir < 4; ++dir)
		{
			Cell nextCell = node.pos.NextCell((DirType)dirOrder[dir]);

			// 인접한 셀이 갈수 없는 영역이면 무시
			if (CanMove(nextCell) == false)
			{
				continue;
			}

			// 방문해야하는 셀이 시적점과 너무 멀면 무시
			int32 depth = nextCell.DeltaLength(start);
			if (depth >= maxDepth)
				continue;

			if (_debugAStar.closedList.count(nextCell) != 0)
			{
				continue;
			}

			// 해당 점수가 정말 최선인지 판단, 뒤늦게 최선의 경로가 발생할수도 있으니
			int g = node.g + 10;
			int h = Heuristic(nextCell, end);
			int f = g + h;

			// 처음 방문하는 노드가 아니라면, 비용 비교
			if (_debugAStar.best.find(nextCell) != _debugAStar.best.end())
			{
				// 다른 경로에서 더 빠른길을 찾았으면 스킵한다.
				if (_debugAStar.best[nextCell] <= f)
				{
					continue;
				}
			}

			// 예약을 진행
			pq.push(PQNode(g, h, nextCell));

			_debugAStar.best[nextCell] = f;
			_debugAStar.parent[nextCell] = node.pos;
			_debugAStar.openList[nextCell] = PQNode(g, h, nextCell);
		}
	}

	Cell newEnd = end;

	// 목적지까지 길이 막혀있다. 목적지까지의 휴리스틱이 제일 좋은게 가장 가까운거다.
	if (found == false)
	{
		// 목적지까지 가장 가까운곳을 넘겨준다.
		int32 bestCostH = INT32_MAX;
		for (auto& iter : _debugAStar.closedList)
		{
			Cell pos = iter;
			int32 costH = _debugAStar.openList[pos].h;
			int32 costG = _debugAStar.openList[pos].g;

			// 현재 cost와 best cost가 같다면, 누적 이동비용이 적은걸 골라준다.
			if (bestCostH == costH)
			{
				int32 bestCostG = _debugAStar.openList[newEnd].g;
				if (costG < bestCostG)
				{
					// 목적지를 현재 위치로 변경해준다.
					newEnd = pos;
				}
			}
			else if (costH < bestCostH)
			{
				newEnd = pos;
				bestCostH = costH;
			}
		}
	}


	findPath.clear();
	Cell pos = newEnd;
	
	// 방문했던 리스트를 찾아가면서 실제 path에 넣어준다.
	while (true)
	{
		if (pos == start)
			break;

		findPath.push_back(pos);

		pos = _debugAStar.parent[pos];
	}

	std::reverse(findPath.begin(), findPath.end());
	return false;
}

void GameScene::CreateExplosionEffect(Vector pos)
{
	DXBitmap* bitmap = ResourceManager::GetInstance()->GetDXBitmap("Explosion");
	if (nullptr == bitmap)
		return;

	int32 frameX, frameY;
	bitmap->GetFrameCount(frameX, frameY);

	AnimInfo info{ 0, 0, frameX, frameY, false, 0.6f};
	Effect* effect = new Effect(pos, "Explosion");
	effect->SetAnimInfo(info);

	// 예약 시스템에 넣는다.
	_reserveAdd.emplace(effect);
}

void GameScene::CreateDropItem(Vector pos, int32 itemId)
{
	const ItemData* itemData = DataManager::GetInstance()->GetItemData(itemId);
	if (nullptr == itemData)
		return;

	DropItem* item = new DropItem(pos, itemData);

	// 예약 시스템에 넣는다.
	_reserveAdd.emplace(item);
}

void GameScene::CreateArrow(Vector pos, DirType dir, Cell dest, int32 attak)
{
	Projectile* arrow = new Projectile(pos);
	arrow->SetProjectileInfo(dir, dest, attak);

	// 예약 시스템에 넣는다.
	_reserveAdd.emplace(arrow);
}

void GameScene::CreateStage(int32 stage)
{
	_currStage = stage;

	// 모든 액터 제거후
	removeAllActor();

	const MapData* mapData = DataManager::GetInstance()->GetData<MapData>(L"MapData");
	if (nullptr == mapData)
		return;

	auto stageFind = mapData->_stage.find(stage);
	if (stageFind == mapData->_stage.end())
		return;

	const StageInfo& stageInfo = stageFind->second;
	{
		fs::path path = ResourceManager::GetInstance()->GetResourcePath() / stageInfo.tileMapPath;
		Map* map = new Map(Vector{ 0, 0 });
		map->LoadTileMap(stage, path.c_str());
		addActor(map);

		_map = map;

		// 맵 정보를 읽어와서 Cell 정보 갱신
		CreateGrid();

		// 포탈 정보 갱신
		Portal* portal = new Portal(Vector{ (float)stageInfo.linkX * GTileSize, (float)stageInfo.linkY * GTileSize });
		addActor(portal);
	}
	{
		Player* player = new Player(Vector{ (float)stageInfo.startX * GTileSize, (float)stageInfo.startY * GTileSize });
		addActor(player);
		
		_player = player;
	}
	{
		deque<Cell> spawnCell = _canMoveCell;

		const MonsterData* monsterData = DataManager::GetInstance()->GetMonsterData(1000);
		Vector initPos((float)stageInfo.startX, (float)stageInfo.startY);
		// 랜덤한 좌표에 몬스터 스폰
		for (int32 i = 0; i < stageInfo.monsterCount; ++i)
		{
			int32 randIndex = rand() % spawnCell.size();
			Cell randomCell = spawnCell[randIndex];

			//Vector pos = initPos;
			//pos.x += (i * GTileSize);
			Vector pos(randomCell.ConvertToPos());
			Enemy* enmey = new Enemy(monsterData, pos);
			addActor(enmey);

			spawnCell.erase(spawnCell.begin() + randIndex);
		}
	}

	InventorySystem::GetInstance()->SceneStart();
}

void GameScene::ShowGameOver()
{
	//_uiMsg.Open();
	//_uiMsg.SetText(L"Game Over!");
}

void GameScene::ToggleVisibleInventory()
{
	if (_uiInven == nullptr)
		return;

	if (_uiInven->IsOpen())
		_uiInven->Close();
	else
		_uiInven->Open();
}

