#include "pch.h"
#include "Scene.h"
#include "ResourceManager.h"
#include "Map.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "ResourceBase.h"
#include "Texture.h"
#include "Sprite.h"
#include "Effect.h"
#include "TimeManager.h"
#include "CollisionManager.h"

bool Scene::drawDebugCell = false;

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

	// 그리드 생성
	_gridCountX = GWinSizeX / _gridSize;
	_gridCountY = GWinSizeY / _gridSize;
	for (int32 i = 0; i < _gridCountX; ++i)
	{
		for (int32 j = 0; j < _gridCountY; ++j)
		{
			Cell cell{ i, j };
			GridInfo gridInfo;
			_grid.emplace(std::move(cell), std::move(gridInfo));
		}
	}
}

void Scene::Destory()
{
	for (auto iter : _actors)
	{
		delete iter;
	}
	_actors.clear();
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
}

void Scene::Render(HDC hdc)
{
	for (auto list : _renderList)
	{
		for (auto actor : list)
		{
			actor->Render(hdc);
		}
	}
	
	// 그리드 디버그용
	if (drawDebugCell)
	{
		drawGrid(hdc);
	}
}

void Scene::drawGrid(HDC hdc)
{
	// 화면 크기와 그리드 크기 설정
	int32 width = GWinSizeX;
	int32 height = GWinSizeY;

	// 빨간색 펜 생성
	HPEN redPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	HPEN oldPen = (HPEN)SelectObject(hdc, redPen);

	// 가로선 그리기
	for (int y = 0; y <= height; y += _gridSize)
	{
		MoveToEx(hdc, 0, y, nullptr); // 시작점 설정
		LineTo(hdc, width, y);        // 끝점까지 선 그리기
	}

	// 세로선 그리기
	for (int x = 0; x <= width; x += _gridSize)
	{
		MoveToEx(hdc, x, 0, nullptr); // 시작점 설정
		LineTo(hdc, x, height);       // 끝점까지 선 그리기
	}

	// 이전 펜 복원 및 새 펜 삭제
	SelectObject(hdc, oldPen);
	DeleteObject(redPen);
}


void Scene::ReserveRemove(Actor* actor)
{
	if (nullptr == actor)
		return;

	if (_reserveRemove.contains(actor))
		return;

	_reserveRemove.emplace(actor);
}

void Scene::UpdateGrid(Actor* actor, Pos prevPos, Pos nextPos)
{
	// 액터의 위치가 변경되었으니 그리드 갱신
	Cell prevCell = Cell::ConvertToCell(prevPos, _gridSize);
	Cell currCell = Cell::ConvertToCell(nextPos, _gridSize);

	// 같으니 갱신 필요 없음
	if (prevCell == currCell)
		return;

	// 이전 셀에서 삭제
	{
		auto find = _grid.find(prevCell);
		if (find != _grid.end())
		{
			auto& gridInfo = find->second;
			auto iter = gridInfo._actors.find(actor);
			if (iter != gridInfo._actors.end())
			{
				gridInfo._actors.erase(iter);
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
			if (false == gridInfo._actors.contains(actor))
			{
				gridInfo._actors.emplace(actor);
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

void Scene::addActor(Actor* actor)
{
	if (nullptr == actor)
		return;

	if (actor->GetRenderLayer() < 0 || actor->GetRenderLayer() >= RenderLayer::RL_Count)
		return;

	actor->Init();
	_actors.emplace(actor);
	_renderList[actor->GetRenderLayer()].emplace_back(actor);

	// 충돌체크 등록
	if (actor->GetCollider() && actor->GetCollider()->CheckCell())
	{
		CollisionManager::GetInstance()->AddCheckCollider(actor->GetCollider());
	}
}

void Scene::removeActor(Actor* actor)
{
	if (nullptr == actor)
		return;

	if (actor->GetRenderLayer() < 0 || actor->GetRenderLayer() >= RenderLayer::RL_Count)
		return;

	UpdateGrid(actor, actor->GetPos(), Pos{-1,-1});

	// 렌더 리스트에서 제거
	{
		auto& list = _renderList[actor->GetRenderLayer()];
		auto iter = std::find(list.begin(), list.end(), actor);
		if (iter != list.end())
		{
			list.erase(iter);
		}
	}

	// 충돌체크 해제
	if (actor->GetCollider() && actor->GetCollider()->CheckCell())
	{
		CollisionManager::GetInstance()->RemoveCheckCollider(actor->GetCollider());
	}

	// 그리드에서 제거
	Cell prevCell = Cell::ConvertToCell(actor->GetPos(), _gridSize);
	auto find = _grid.find(prevCell);
	if (find != _grid.end())
	{
		auto& gridInfo = find->second;
		auto iter = gridInfo._actors.find(actor);
		if (iter != gridInfo._actors.end())
		{
			gridInfo._actors.erase(iter);
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
