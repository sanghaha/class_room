#include "pch.h"
#include "PlayerScene.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "Game.h"
#include "StageLoader.h"
#include "Player.h"
#include "Ball.h"

PlayerScene::PlayerScene()
{
}

PlayerScene::~PlayerScene()
{
}

void PlayerScene::Init()
{
	Super::Init();

	ResourceManager::GetInstance()->LoadTexture(L"GameBG", L"background_game.bmp");
	ResourceManager::GetInstance()->LoadTexture(L"Player", L"Warrior_Blue.bmp", 6, 8, RGB(255, 0, 255), true);
	ResourceManager::GetInstance()->LoadTexture(L"Block", L"201-217_block.bmp", 3, 6);
	ResourceManager::GetInstance()->LoadTexture(L"TileSelector", L"Tile_Selector.bmp", 1, 1, RGB(255,0,255));

	loadStage();
}

void PlayerScene::Destory()
{
	Super::Destory();
}

void PlayerScene::Update(float deltaTime)
{
	Super::Update(deltaTime);

	if (InputManager::GetInstance()->GetButtonDown(KeyType::Escape))
		Game::GetInstance()->ChangeLobbyScene();
}

void PlayerScene::Render(HDC renderTarget)
{
	Super::Render(renderTarget);
}

void PlayerScene::loadStage()
{
	wstring fileName = L"a_star.stage";
	fs::path directory = ResourceManager::GetInstance()->GetResourcePath() / L"Stage/" / fileName;

	std::wifstream file(directory);
	if (!file.is_open())
		return;

	StageLoader loader;
	loader.Load(this, file);
	file.close();

	// Ball actor in _reserveAdd -> replace with Player at same position
	Vector playerStartPos(BLOCK_SIZE, BLOCK_SIZE);
	Actor* ballActor = nullptr;
	for (Actor* a : _reserveAdd)
	{
		if (a->GetActorType() == ActorType::AT_BALL)
		{
			playerStartPos = a->GetPos();
			ballActor = a;
			break;
		}
	}
	if (ballActor)
	{
		Cell ballCell = Cell::ConvertToCell(ballActor->GetPos(), BLOCK_SIZE);
		auto it = _grid.find(ballCell);
		if (it != _grid.end())
			it->second._actors.erase(ballActor);

		_reserveAdd.erase(ballActor);
		delete ballActor;
	}
}
