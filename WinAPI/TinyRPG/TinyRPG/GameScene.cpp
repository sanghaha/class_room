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

		// 맵 정보를 읽어와서 Cell 정보 갱신
		CreateGrid();
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

			_grid.emplace(std::move(cell), std::move(gridInfo));
		}
	}
}

void GameScene::initTimer()
{
}

bool GameScene::CanMove(Pos pos)
{
	Cell cell = Cell::ConvertToCell(pos, GTileSize);
	auto find = _grid.find(cell);
	if (find != _grid.end())
	{
		return find->second.canMoveCell;
	}
	return false;
}
