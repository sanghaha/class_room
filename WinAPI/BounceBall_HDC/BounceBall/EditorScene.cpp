#include "pch.h"
#include "EditorScene.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "StageLoader.h"
#include "Game.h"
#include "Background.h"
#include "Ball.h"
#include "Block.h"
#include "Star.h"


EditorScene::EditorScene()
{
}

EditorScene::~EditorScene()
{
}

void EditorScene::Init()
{
	Super::Init();

	ResourceManager::GetInstance()->LoadDXBitmap(L"GameBG", L"background_game.bmp");
	ResourceManager::GetInstance()->LoadDXBitmap(L"Ball", L"001-008_ball.bmp", 2, 4);
	ResourceManager::GetInstance()->LoadDXBitmap(L"Block", L"201-217_block.bmp", 3, 6);
	ResourceManager::GetInstance()->LoadDXBitmap(L"Star", L"101-103_star.bmp", 1, 3);

	InputManager::GetInstance()->SetEventMouseWheel([this](int32 delta) { OnMouseWheel(delta); });

	// bg
	{
		Background* bg = new Background(Vector(0, 0));
		ReserveAdd(bg);
	}
	// ball
	{
		SELECT_MODE mode = SELECT_MODE::SM_BALL;
		SpriteActor* actor = new Ball(Vector(0, 0));
		actor->Init();

		_editActorInfo[mode].tempActor = actor;
		_editActorInfo[mode].spriteCount = 0;
		_editActorInfo[mode].spriteMax = 8;
		_editActorInfo[mode].spriteName = "Ball";
	}
	// block
	{
		SELECT_MODE mode = SELECT_MODE::SM_BLOCK;
		SpriteActor* actor = new Block(Vector(0, 0));
		actor->Init();

		_editActorInfo[mode].tempActor = actor;
		_editActorInfo[mode].spriteCount = 0;
		_editActorInfo[mode].spriteMax = 17;
		_editActorInfo[mode].spriteName = "Block";
	}
	// star
	{
		SELECT_MODE mode = SELECT_MODE::SM_STAR;
		SpriteActor* actor = new Star(Vector(0, 0));
		actor->Init();

		_editActorInfo[mode].tempActor = actor;
		_editActorInfo[mode].spriteCount = 0;
		_editActorInfo[mode].spriteMax = 3;
		_editActorInfo[mode].spriteName = "Star";
	}
}

void EditorScene::Destory()
{
	InputManager::GetInstance()->SetEventMouseWheel(nullptr);
}

void EditorScene::Update(float deltaTime)
{
	Super::Update(deltaTime);

	if (InputManager::GetInstance()->GetButtonDown(KeyType::LeftMouse))
	{
		POINT mousePos = InputManager::GetInstance()->GetMousePos();
		int32 x = (mousePos.x / BLOCK_SIZE);
		int32 y = (mousePos.y / BLOCK_SIZE);

		Cell cell{ x, y };
		const GridInfo& gridInfo = GetGridInfo(cell);
		if(gridInfo._actors.empty())
		{
			x = x * BLOCK_SIZE + (int32)(BLOCK_SIZE * 0.5f);
			y = y * BLOCK_SIZE + (int32)(BLOCK_SIZE * 0.5f);

			SpriteActor* newActor = nullptr;
			if (_currMode == SELECT_MODE::SM_BLOCK)
			{
				newActor = new Block(Vector(x, y));
			}
			else if (_currMode == SELECT_MODE::SM_BALL)
			{
				for (auto iter : _actors)
				{
					if (dynamic_cast<Ball*>(iter))
					{
						ReserveRemove(iter);
					}
				}
				newActor = new Ball(Vector(x, y));
			}
			else if (_currMode == SELECT_MODE::SM_STAR)
			{
				newActor = new Star(Vector(x, y));
			}

			ReserveAdd(newActor);

			EditActorInfo* info = GetCurrModeActor();
			SetSpriteInfo(newActor, info->spriteName, info->spriteCount);
		}
	}
	else if (InputManager::GetInstance()->GetButtonDown(KeyType::RightMouse))
	{
		POINT mousePos = InputManager::GetInstance()->GetMousePos();
		int32 x = (mousePos.x / BLOCK_SIZE);
		int32 y = (mousePos.y / BLOCK_SIZE);

		Cell cell{ x, y };
		const GridInfo& gridInfo = GetGridInfo(cell);
		if (false == gridInfo._actors.empty())
		{
			Actor* actor = *(gridInfo._actors.begin());
			actor->Destroy();
		}
	}
	else if (InputManager::GetInstance()->GetButtonDown(KeyType::F1))
	{
		_currMode = SELECT_MODE::SM_BLOCK;
	}
	else if (InputManager::GetInstance()->GetButtonDown(KeyType::F2))
	{
		_currMode = SELECT_MODE::SM_BALL;
	}
	else if (InputManager::GetInstance()->GetButtonDown(KeyType::F3))
	{
		_currMode = SELECT_MODE::SM_STAR;
	}
	else if (InputManager::GetInstance()->GetButtonDown(KeyType::S))
	{
		Save();
	}
	else if (InputManager::GetInstance()->GetButtonDown(KeyType::L))
	{
		Load();
	}
}

void EditorScene::Render(HDC renderTarget)
{
	Super::Render(renderTarget);
}

void EditorScene::Save()
{
	OPENFILENAME ofn;
	wchar_t szFileName[MAX_PATH] = L"";

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = Game::GetInstance()->GetHwnd();
	ofn.lpstrFilter = L"스테이지 파일 (*.stage)\0*.stage\0모든 파일 (*.*)\0*.*\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	ofn.lpstrDefExt = L"stage";

	if (GetSaveFileName(&ofn)) 
	{
		// 파일 이름이 선택되었으면 저장
		std::wstring fileName = szFileName;

		std::wofstream file(fileName);
		if (file.is_open()) 
		{
			// 데이터 저장
			for (auto iter : _actors)
			{
				iter->SaveActor(file);
			}

			file.close();
			MessageBox(Game::GetInstance()->GetHwnd(), L"맵이 저장되었습니다.", L"저장 완료", MB_OK | MB_ICONINFORMATION);
		}
		else {
			MessageBox(Game::GetInstance()->GetHwnd(), L"파일을 저장할 수 없습니다.", L"오류", MB_OK | MB_ICONERROR);
		}
	}
}

void EditorScene::Load()
{
	OPENFILENAME ofn;
	wchar_t szFileName[MAX_PATH] = L"";

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = Game::GetInstance()->GetHwnd();
	ofn.lpstrFilter = L"스테이지 파일 (*.stage)\0*.stage\0모든 파일 (*.*)\0*.*\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	ofn.lpstrDefExt = L"stage";

	if (GetOpenFileName(&ofn)) 
	{
		// 파일 이름이 선택되었으면 로드
		std::wstring fileName = szFileName;

		std::wifstream file(fileName);
		if (file.is_open()) 
		{
			StageLoader loader;
			loader.Load(this, file);

			file.close();
			MessageBox(Game::GetInstance()->GetHwnd(), L"맵이 로드되었습니다.", L"로드 완료", MB_OK | MB_ICONINFORMATION);
		}
		else {
			MessageBox(Game::GetInstance()->GetHwnd(), L"파일을 로드할 수 없습니다.", L"오류", MB_OK | MB_ICONERROR);
		}
	}
}

void EditorScene::OnMouseWheel(int32 delta)
{
	EditActorInfo* info = GetCurrModeActor();

	if (delta > 0)
	{
		info->spriteCount++;
		if (info->spriteCount >= info->spriteMax) info->spriteCount = 0;
	}
	else
	{
		info->spriteCount--;
		if(info->spriteCount <  0) info->spriteCount = info->spriteMax - 1;
	}

	SetSpriteInfo(info->tempActor, info->spriteName, info->spriteCount);
}

void EditorScene::SetSpriteInfo(SpriteActor* actor, string spriteName, int32 count)
{
	string key = std::format("{0}_{1}", spriteName, count);
	const SpriteInfo* info = ResourceManager::GetInstance()->GetSpriteInfo(key);
	if (info && actor)
	{
		actor->SetSpriteIndex(info->indexX, info->indexY);
	}
}

EditActorInfo* EditorScene::GetCurrModeActor()
{
	return &_editActorInfo[_currMode];
}
