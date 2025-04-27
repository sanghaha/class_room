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
#include "DataManager.h"
#include "MapData.h"

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
	ResourceManager::GetInstance()->LoadPNGSprite(L"Torch_Red", L"Monster/Torch_Red.png", 7, 5);
	ResourceManager::GetInstance()->LoadPNGSprite(L"Explosion", L"Effect/Explosions.png", 9, 1);
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

		// 맵 정보를 읽어와서 Cell 정보 갱신
		CreateGrid();
	}
	{
		Sprite* sprite = ResourceManager::GetInstance()->GetSprite(L"Warrior_Blue");
		Player* player = new Player(Vector{ 160, 160 });
		player->SetTexture(sprite);
		addActor(player);

		_player = player;
	}
	{
		Sprite* sprite = ResourceManager::GetInstance()->GetSprite(L"Torch_Red");
		vector<Cell> spawnCell =_canMoveCell;

		// 랜덤한 좌표에 몬스터 스폰
		for (int32 i = 0; i < mapData->_monsterCount; ++i)
		{
			int32 randIndex = rand() % spawnCell.size();
			Cell randomCell = spawnCell[randIndex];

			Enemy* enmey = new Enemy(randomCell.ConvertToPos());
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
		// 누군가 있다면 못감
		if (find->second._actors.size() != 0)
			return false;

		return find->second.canMoveCell;
	}
	return false;
}

void GameScene::CreateExplosionEffect(Vector pos)
{
	EffectExplosion* effect = new EffectExplosion(pos);

	// 예약 시스템에 넣는다.
	_reserveAdd.emplace(effect);
}
