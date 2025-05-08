#include "pch.h"
#include "Portal.h"
#include "TransitionManager.h"
#include "Game.h"
#include "GameScene.h"
#include "DataManager.h"
#include "MapData.h"

Portal::Portal(Vector pos) : Super(pos)
{
}

Portal::~Portal()
{
}

void Portal::Init()
{
	Super::Init();
}

void Portal::Update(float deltaTime)
{
}

void Portal::Render(ID2D1RenderTarget* renderTarget)
{
}

RenderLayer Portal::GetRenderLayer()
{
	return RenderLayer::RL_Effect;
}

void Portal::EnterNextStage()
{

	TransitionManager::GetInstance()->Start(TransitionType::Fragment, []() 
		{
			// 화면 전환
			const MapData* mapData = DataManager::GetInstance()->GetData<MapData>(L"MapData");
			if (nullptr == mapData)
				return;

			int8 currStage = Game::GetGameScene()->GetCurrStage();
			auto find = mapData->_stage.find(currStage);
			if (find != mapData->_stage.end())
			{
				Game::GetGameScene()->CreateStage(find->second.linkMapId);
			}
		}
	);
}
