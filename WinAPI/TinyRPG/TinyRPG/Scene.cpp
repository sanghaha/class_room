#include "pch.h"
#include "Scene.h"
#include "ResourceManager.h"
#include "Map.h"
#include "Player.h"
#include "Enemy.h"
#include "Texture.h"
#include "Sprite.h"
#include "Effect.h"
#include "TimeManager.h"
#include "Game.h"
#include "InputManager.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::Init()
{
	// 리소스 로드
	loadResources();

	// 오브젝트 생성
	createObjects();

	// 타이머 추가
	initTimer();

}

void Scene::Update(float deltaTime)
{
	for (auto actor : _actors)
	{
		actor->Update(deltaTime);
	}

	// 추가된 예약 처리
	for (auto actor : _reserveAdd)
	{
		// 예약된 액터를 렌더링 리스트에 추가한다.
		addActor(actor);
	}
	_reserveAdd.clear(); // 추가 후 예약 리스트 초기화

	// 삭제된 예약 처리
	for (Actor* actor : _reserveRemove)
	{
		removeActor(actor);
	}

	_reserveRemove.clear(); // 삭제 후 예약 리스트 초기화

	if (InputManager::GetInstance()->GetButtonDown(KeyType::F1))
	{
		_drawGridCell = !_drawGridCell;
	}
}

void Scene::Render(ID2D1HwndRenderTarget* renderTarget)
{
	for (auto list : _renderList)
	{
		for (auto actor : list)
		{
			actor->Render(renderTarget);
		}
	}

	// 그리드 디버그용
	if (_drawGridCell)
	{
		drawGrid(renderTarget);
	}
}

void Scene::drawGrid(ID2D1HwndRenderTarget* renderTarget)
{
	// 화면 크기와 그리드 크기 설정
	int32 width = _gridCountX * GTileSize;
	int32 height = _gridCountY * GTileSize;

	auto brush = ResourceManager::GetInstance()->GetBrush(BrushColor::Red);

	// 가로선 그리기
	for (int y = 0; y <= height; y += GTileSize)
	{
		Vector renderPos1 = Game::ConvertRenderPos(Vector((float)0, (float)y));
		Vector renderPos2 = Game::ConvertRenderPos(Vector((float)width, (float)y));
		
		D2D1_POINT_2F start = D2D1::Point2F(renderPos1.x, renderPos1.y);
		D2D1_POINT_2F end = D2D1::Point2F(renderPos2.x, renderPos2.y);
		renderTarget->DrawLine(start, end, brush, 1.0f);
	}

	// 세로선 그리기
	for (int x = 0; x <= width; x += GTileSize)
	{
		Vector renderPos1 = Game::ConvertRenderPos(Vector((float)x, (float)0));
		Vector renderPos2 = Game::ConvertRenderPos(Vector((float)x, (float)height));
		
		D2D1_POINT_2F start = D2D1::Point2F(renderPos1.x, renderPos1.y);
		D2D1_POINT_2F end = D2D1::Point2F(renderPos2.x, renderPos2.y);
		renderTarget->DrawLine(start, end, brush, 1.0f);
	}
}


void Scene::ReserveRemove(Actor* actor)
{
	if (nullptr == actor)
		return;

	if (_reserveRemove.contains(actor))
		return;

	_reserveRemove.emplace(actor);
}

Player* Scene::GetPlayer()
{
	return _player;
}

void Scene::UpdateGrid(Actor* actor, Cell prevCell, Cell currCell)
{
	// 액터의 위치가 변경되었으니 그리드 갱신
	// 같으니 갱신 필요 없음
	if (prevCell == currCell)
		return;

	// 이전 셀에서 삭제
	{
		auto find = _grid.find(prevCell);
		if (find != _grid.end())
		{
			auto& gridInfo = find->second;
			auto findActor = gridInfo._actorsInCell.find(actor);
			if (findActor != gridInfo._actorsInCell.end())
			{
				if ((*findActor)->IsBlockingCell())
					gridInfo.blockedCount++;
				gridInfo._actorsInCell.erase(findActor);

				// 기존에 있던 actor들과 충돌 끝낫다고 알려준다.
				for (auto iter : gridInfo._actorsInCell)
				{
					iter->OnEndOverlapActor(actor);
					actor->OnEndOverlapActor(iter);
				}
			}
		}
	}

	// 현재 추가하려는 셀이 범위 밖이면 무시
	if (currCell.index_X < 0 || currCell.index_X >= _gridCountX || currCell.index_Y < 0 || currCell.index_Y >= _gridCountY)
		return;

	// 현재 셀에 추가
	{
		auto find = _grid.find(currCell);
		if (find != _grid.end())
		{
			auto& gridInfo = find->second;
			if (false == gridInfo._actorsInCell.contains(actor))
			{
				// 처음 진입하는 셀이니깐, 기존에 있던 actor들과 충돌되었다고 알려준다.
				for (auto iter : gridInfo._actorsInCell)
				{
					actor->OnBeginOverlapActor(iter);
					iter->OnBeginOverlapActor(actor);
				}

				gridInfo._actorsInCell.emplace(actor);

				if (actor->IsBlockingCell())
					gridInfo.blockedCount--;
			}
		}
	}
}

const GridInfo& Scene::GetGridInfo(const Cell& cell)
{
	auto find = _grid.find(cell);
	if (find != _grid.end())
	{
		return find->second;
	}

	static GridInfo emptyGridInfo;
	return emptyGridInfo;
}

bool Scene::IsCulling(Vector pos) const
{
	// 해당 위치가 카메라 밖인지 확인
	Vector cameraMin;
	cameraMin.x = _cameraPos.x - GWinSizeX / 2 - GTileSize;
	cameraMin.y = _cameraPos.y - GWinSizeY / 2 - GTileSize;

	Vector cameraMax;
	cameraMax.x = _cameraPos.x + GWinSizeX / 2 + GTileSize;
	cameraMax.y = _cameraPos.y + GWinSizeY / 2 + GTileSize;

	if (pos.x < cameraMin.x)
		return true;

	if (pos.x > cameraMax.x)
		return true;

	if (pos.y < cameraMin.y)
		return true;

	if (pos.y > cameraMax.y)
		return true;

	return false;
}

void Scene::addActor(Actor* actor)
{
	if (nullptr == actor)
		return;

	if (actor->GetRenderLayer() < 0 || actor->GetRenderLayer() >= RenderLayer::RL_Count)
		return;

	actor->Init();
	_actors.emplace(actor);
	_renderList[actor->GetRenderLayer()].emplace_back(actor);

	if (RenderLayer::RL_Player == actor->GetRenderLayer())
	{
		_player = dynamic_cast<Player*>(actor);
	}
}

void Scene::removeActor(Actor* actor)
{
	if (nullptr == actor)
		return;

	if (actor->GetRenderLayer() < 0 || actor->GetRenderLayer() >= RenderLayer::RL_Count)
		return;

	if (RenderLayer::RL_Player == actor->GetRenderLayer())
	{
		_player = nullptr;
	}

	UpdateGrid(actor, actor->GetPosCell(), Cell{ -1,-1 });

	// 렌더 리스트에서 제거
	{
		auto& list = _renderList[actor->GetRenderLayer()];
		auto iter = std::find(list.begin(), list.end(), actor);
		if (iter != list.end())
		{
			list.erase(iter);
		}
	}

	// 실제 actor set 에서 제거
	{
		auto iter = _actors.find(actor);
		if (iter != _actors.end())
		{
			_actors.erase(iter);
			delete actor;
		}
	}
}
