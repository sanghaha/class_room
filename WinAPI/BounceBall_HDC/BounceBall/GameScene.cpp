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
#include "Effect.h"
#include "UIButton.h"
#include "UIImage.h"
#include "SoundManager.h"
#include "RectCollider.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::Init()
{
	Super::Init();

	ResourceManager::GetInstance()->LoadTexture(L"GameBG", L"background_game.bmp");
	ResourceManager::GetInstance()->LoadTexture(L"Ball", L"001-008_ball.bmp", 2, 4);
	ResourceManager::GetInstance()->LoadTexture(L"Block", L"201-217_block.bmp", 3, 6);
	ResourceManager::GetInstance()->LoadTexture(L"Star", L"101-103_star.bmp", 1, 3);
	ResourceManager::GetInstance()->LoadTexture(L"EatStarEffect", L"Eat_Star.bmp", 23, 1);
	ResourceManager::GetInstance()->LoadTexture(L"DeadBall", L"Deadball.bmp", 42, 1);
	ResourceManager::GetInstance()->LoadTexture(L"LevelComplete", L"level_complete.png");
	ResourceManager::GetInstance()->LoadTexture(L"NextStageButton", L"next_stage.png");

	ResourceManager::GetInstance()->LoadSound(L"B_Bauns", L"Sound\\B_Bauns.wav");
	ResourceManager::GetInstance()->LoadSound(L"Bauns", L"Sound\\Bauns.wav");
	ResourceManager::GetInstance()->LoadSound(L"DeadS", L"Sound\\DeadS.wav");
	ResourceManager::GetInstance()->LoadSound(L"Select", L"Sound\\Select.wav");
	ResourceManager::GetInstance()->LoadSound(L"Star", L"Sound\\Star.wav");

	_completeImg = _ui.CreateImage(Vector(20, 20), L"LevelComplete");
	_completeImg->SetOpen(false);

	_nextStageButton = _ui.CreateButton(Vector(20, 300), L"NextStageButton");
	_nextStageButton->SetOpen(false);
	_nextStageButton->SetClickEvent([this]() { onClickNextStageButton(); });

	loadStage(_currStage);
	createCollisionMask();
}

void GameScene::Update(float deltaTime)
{
	Super::Update(deltaTime);
}

void GameScene::Render(HDC renderTarget)
{
	Super::Render(renderTarget);

	uint32 fps = TimeManager::GetInstance()->GetFps();
	float deltaTime = TimeManager::GetDeltaTime();
}

bool GameScene::CheckCollision(class Ball* ball, Vector start, Vector end, Vector& outNormal, Vector& outPos)
{
	float tMin = FLT_MAX;
	Actor* closestActor = nullptr;

	for (auto iter : _actors)
	{
		ActorType actorType = iter->GetActorType();
		bool block = (COLLISION_BIT_MASK_BLOCK & ((uint64)1 << actorType));
		bool overlap = (COLLISION_BIT_MASK_OVERLAP & ((uint64)1 << actorType));

		// �浹üũ ���ϴ� �༮��
		if (!block && !overlap)
			continue;

		if (!iter->GetCollider())
			continue;

		MyRect rect = *iter->GetCollider()->GetCollisionRect();
		rect.left -= BALL_SIZE * 0.5f;
		rect.right += BALL_SIZE * 0.5f;
		rect.top -= BALL_SIZE * 0.5f;
		rect.bottom += BALL_SIZE * 0.5f;

		float t = 0.f;
		Vector normal;
		Vector hitPos;
		Vector dir = end - start;
		dir.Normalize();
		//start -= (dir * 2.0f);
		//if (rect && CheckCircleAABB(end.x, end.y, ball->GetRadius(), *rect, outNormal, outPos))
		if (LineIntersectsAABB(start, end, rect, normal, hitPos, t))
		//if(rect && IntersectSegmentRect(start, end, *rect, outNormal, outPos))
		{
			if (t < tMin)
			{
				outNormal = normal;
				outPos = hitPos;
				tMin = t;
				closestActor = iter;
			}
		}
	}

	if (closestActor != nullptr)
	{
		// ���� ���𰡰� �浹�Ǿ���, overlap ó��
		ball->OnBeginOverlapActor(closestActor);
		return true;
	}
	return false;
}

void GameScene::CreateEffect(Vector pos, string spriteName)
{
	Effect* effect = new Effect(pos, spriteName);
	ReserveAdd(effect);
}

void GameScene::AddStarCount(Vector pos)
{
	CreateEffect(pos, "EatStarEffect");

	++_curStarCount;

	//ResourceManager::GetInstance()->PlayWAVSound("Star.wav");
	SoundManager::GetInstance()->Play(L"Star");

	if (_curStarCount >= _maxStarCount)
	{
		clearStage();
	}
}

void GameScene::Dead(Vector pos)
{
	// ���� ��ġ�� ����
	// �� ���󺹱�
	// �������� ��ε�
	AddPostUpdateAction([this, pos]() 
		{
			loadStage(_currStage); 

			Vector effectPos = pos;
			effectPos.y -= 15;
			CreateEffect(effectPos, "DeadBall");
			//ResourceManager::GetInstance()->PlayWAVSound("DeadS.wav");
			SoundManager::GetInstance()->Play(L"DeadS");
		});
}

bool GameScene::loadStage(int32 stage)
{
	wstring fileName = std::format(L"stage_{0}.stage", stage);
	fs::path directory = ResourceManager::GetInstance()->GetResourcePath() / L"Stage/" / fileName;

	std::wifstream file(directory);
	if (file.is_open())
	{
		StageLoader loader;
		_maxStarCount = loader.Load(this, file);
		_curStarCount = 0;

		file.close();

		return true;
	}

	return false;
}

void GameScene::clearStage()
{
	if(_completeImg)
		_completeImg->SetOpen(true);

	if(_nextStageButton)
		_nextStageButton->SetOpen(true);
}

void GameScene::createCollisionMask()
{
	// BLOCK ���� �浹üũ
	COLLISION_BIT_MASK_BLOCK |= (1 << ActorType::AT_BLOCK);

	// STAR �浹 �˻縸 �ϰ� ��ĥ�� �ִ�.
	COLLISION_BIT_MASK_OVERLAP |= (1 << ActorType::AT_STAR);
}

void GameScene::onClickNextStageButton()
{
	if (loadStage(_currStage + 1))
	{
		if (_completeImg)
			_completeImg->SetOpen(false);

		if (_nextStageButton)
			_nextStageButton->SetOpen(false);

		_currStage++;
	}
}

