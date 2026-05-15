#include "pch.h"
#include "GameScene.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "TimeManager.h"
#include "UIPanel.h"
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
	ResourceManager::GetInstance()->LoadTexture(L"LevelComplete", L"level_complete.bmp", 1, 1, RGB(255, 0, 255));
	ResourceManager::GetInstance()->LoadTexture(L"NextStageButton", L"next_stage.bmp", 1, 1, RGB(255, 0, 255));

	ResourceManager::GetInstance()->LoadSound(L"B_Bauns", L"Sound\\B_Bauns.wav");
	ResourceManager::GetInstance()->LoadSound(L"Bauns", L"Sound\\Bauns.wav");
	ResourceManager::GetInstance()->LoadSound(L"DeadS", L"Sound\\DeadS.wav");
	ResourceManager::GetInstance()->LoadSound(L"Select", L"Sound\\Select.wav");
	ResourceManager::GetInstance()->LoadSound(L"Star", L"Sound\\Star.wav");

	_completePanel = new UIPanel(Vector(0, 0));
	_completePanel->Init();
	_completePanel->CreateImage(Vector(20, 20), L"LevelComplete");
	
	UIButton* nextBtn = _completePanel->CreateButton(Vector(20, 300), L"NextStageButton");
	nextBtn->SetClickEvent([this]() { onClickNextStageButton(); });

	_completePanel->Hide();

	loadStage(_currStage);
	createCollisionMask();
}

void GameScene::Destory()
{
	Super::Destory();

	SAFE_DELETE(_completePanel);
}

void GameScene::Update(float deltaTime)
{
	Super::Update(deltaTime);

	if (_completePanel)
		_completePanel->Update();
}

void GameScene::Render(HDC renderTarget)
{
	Super::Render(renderTarget);

	uint32 fps = TimeManager::GetInstance()->GetFps();
	float deltaTime = TimeManager::GetDeltaTime();

	if (_completePanel)
		_completePanel->Render(renderTarget);
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

		// 충돌체크 안하는 녀석들
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
		// 공과 무언가가 충돌되었고, overlap 처리
		ball->OnBeginOverlapActor(closestActor);
		return true;
	}
	return false;
}

bool GameScene::CheckCollision_ClosestPoint(class Ball* ball, Vector ballPos, float radius, Vector& outNormal, Vector& outPos)
{
	Actor* closestActor = nullptr;
	float maxPenetration = -1.f;

	for (auto iter : _actors)
	{
		ActorType actorType = iter->GetActorType();
		bool block = (COLLISION_BIT_MASK_BLOCK & ((uint64)1 << actorType));
		bool overlap = (COLLISION_BIT_MASK_OVERLAP & ((uint64)1 << actorType));

		if (!block && !overlap)
			continue;

		if (!iter->GetCollider())
			continue;

		MyRect rect = *iter->GetCollider()->GetCollisionRect();

		float closestX = std::max((float)rect.left, std::min(ballPos.x, (float)rect.right));
		float closestY = std::max((float)rect.top, std::min(ballPos.y, (float)rect.bottom));

		float distanceX = ballPos.x - closestX;
		float distanceY = ballPos.y - closestY;
		float distanceSquared = (distanceX * distanceX) + (distanceY * distanceY);

		if (distanceSquared <= radius * radius)
		{
			float distance = sqrt(distanceSquared);
			float penetration = radius - distance;

			if (penetration > maxPenetration)
			{
				maxPenetration = penetration;
				closestActor = iter;

				if (distance > 0.0001f)
				{
					// 축-정렬 법선: 절댓값이 큰 축 방향으로 스냅 (모서리 충돌 시 대각선 방지)
					if (std::abs(distanceX) >= std::abs(distanceY))
						outNormal = Vector(distanceX > 0.f ? 1.f : -1.f, 0.f);
					else
						outNormal = Vector(0.f, distanceY > 0.f ? 1.f : -1.f);
				}
				else
				{
					// 공의 중심이 사각형 내부: 최소 침투 깊이 방향으로 밀어냄
					float dLeft   = ballPos.x - rect.left;
					float dRight  = rect.right  - ballPos.x;
					float dTop    = ballPos.y - rect.top;
					float dBottom = rect.bottom - ballPos.y;
					float minD = std::min({ dLeft, dRight, dTop, dBottom });

					if      (minD == dLeft)   outNormal = Vector(-1.f,  0.f);
					else if (minD == dRight)  outNormal = Vector( 1.f,  0.f);
					else if (minD == dTop)    outNormal = Vector( 0.f, -1.f);
					else                      outNormal = Vector( 0.f,  1.f);
				}

				outPos = ballPos + (outNormal * penetration);
			}
		}
	}

	if (closestActor != nullptr)
	{
		ball->OnBeginOverlapActor(closestActor);

		ActorType actorType = closestActor->GetActorType();
		if (COLLISION_BIT_MASK_BLOCK & ((uint64)1 << actorType))
			return true;
	}

	return false;
}

void GameScene::CreateEffect(Vector pos, string spriteName)
{
	Effect* effect = new Effect(pos, spriteName);
	effect->Init();
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
		AddPostUpdateAction([this]()
			{
				clearStage();
			});
	}
}

void GameScene::Dead(Vector pos)
{
	// 시작 위치로 변경
	// 별 원상복귀
	// 스테이지 재로드
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
	if (_completePanel)
		_completePanel->Show();
}

void GameScene::createCollisionMask()
{
	// BLOCK 과는 충돌체크
	COLLISION_BIT_MASK_BLOCK |= (1 << ActorType::AT_BLOCK);

	// STAR 충돌 검사만 하고 겹칠수 있다.
	COLLISION_BIT_MASK_OVERLAP |= (1 << ActorType::AT_STAR);
}

void GameScene::onClickNextStageButton()
{
	if (loadStage(_currStage + 1))
	{
		if (_completePanel)
			_completePanel->Hide();

		_currStage++;
	}
}