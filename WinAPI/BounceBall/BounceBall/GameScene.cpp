#include "pch.h"
#include "GameScene.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "TimeManager.h"
#include "UIManager.h"
#include "InputManager.h"
#include "Game.h"
#include "DataManager.h"
#include "Ball.h"
#include "Block.h"
#include "Background.h"
#include "StageLoader.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::Init()
{
	Super::Init();

	ResourceManager::GetInstance()->LoadDXBitmap(L"GameBG", L"background_game.bmp");
	ResourceManager::GetInstance()->LoadDXBitmap(L"Ball", L"001-008_ball.bmp", 2, 4);
	ResourceManager::GetInstance()->LoadDXBitmap(L"Block", L"201-217_block.bmp", 3, 6);
	ResourceManager::GetInstance()->LoadDXBitmap(L"Star", L"101-103_star.bmp", 1, 3);

	loadStage(L"stage_1.stage");
}

void GameScene::Update(float deltaTime)
{
	Super::Update(deltaTime);
}

void GameScene::Render(ID2D1RenderTarget* renderTarget)
{
	Super::Render(renderTarget);

	uint32 fps = TimeManager::GetInstance()->GetFps();
	float deltaTime = TimeManager::GetDeltaTime();

	auto brush = ResourceManager::GetInstance()->GetBrush(BrushColor::White);
	auto font = ResourceManager::GetInstance()->GetFont(FontSize::FONT_12);

	{
		POINT mousePos = InputManager::GetInstance()->GetMousePos();
		wstring str = std::format(L"Mouse({0}, {1})", mousePos.x, mousePos.y);
		renderTarget->DrawTextW(
			str.c_str(),
			(uint32)str.size(),
			font,
			D2D1::RectF(300, 10, 600, 200),
			brush
		);
	}

	{
		wstring str = std::format(L"FPS({0}), DT({1})", fps, deltaTime);
		renderTarget->DrawTextW(
			str.c_str(),
			(uint32)str.size(),
			font,
			D2D1::RectF(5, 10, 600, 200),
			brush
		);
	}
}

bool GameScene::CheckCollision(class Ball* ball, Vector start, Vector end, Vector& outNormal, float& depth)
{
	for (auto iter : _actors)
	{
		Rect* rect = iter->GetCollisionRect();
		if (rect)
		{
			rect->left -= BALL_SIZE * 0.5f;
			rect->right += BALL_SIZE * 0.5f;
			rect->top -= BALL_SIZE * 0.5f;
			rect->bottom += BALL_SIZE * 0.5f;
		}
		if (rect && LineIntersectsAABB(start, end, *rect, outNormal, depth))
		{
			// 공과 무언가가 충돌되었고, overlap 처리
			if (ball->OnBeginOverlapActor(iter))
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	}
	return false;
}

void GameScene::loadStage(std::wstring fileName)
{
	fs::path directory = ResourceManager::GetInstance()->GetResourcePath() / L"Stage/" / fileName;

	std::wifstream file(directory);
	if (file.is_open())
	{
		StageLoader loader;
		loader.Load(this, file);

		file.close();
	}
}

