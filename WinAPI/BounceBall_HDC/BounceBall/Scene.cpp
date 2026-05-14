#include "pch.h"
#include "Scene.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "TimeManager.h"
#include "Game.h"
#include "InputManager.h"
#include "Actor.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::Init()
{
	_gridCountX = GWinSizeX / BLOCK_SIZE;
	_gridCountY = GWinSizeY / BLOCK_SIZE;
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
	removeAllActor();
}

void Scene::Update(float deltaTime)
{
	for (auto actor : _actors)
	{
		actor->Update(deltaTime);
	}

	// 추가된 액터 처리
	for (auto actor : _reserveAdd)
	{
		// 예비용 액터를 실체 리스트에 추가한다.
		addActor(actor);
	}
	_reserveAdd.clear(); // 추가 할 액터 리스트 초기화

	// 삭제할 액터 처리
	for (Actor* actor : _reserveRemove)
	{
		removeActor(actor);
	}

	_reserveRemove.clear(); // 삭제 할 액터 리스트 초기화

	// 후처리 액션 수행
	for (auto& action : _postUpdateActions)
	{
		action();
	}
	_postUpdateActions.clear();
}

void Scene::Render(HDC renderTarget)
{
	for (auto list : _renderList)
	{
		for (auto actor : list)
		{
			actor->Render(renderTarget);
		}
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

void Scene::ReserveAdd(Actor* actor)
{
	if (nullptr == actor)
		return;

	if (_reserveAdd.contains(actor))
		return;

	actor->Init();
	_reserveAdd.emplace(actor);
}

void Scene::AddPostUpdateAction(std::function<void()> action)
{
	_postUpdateActions.emplace_back(action);
}

void Scene::UpdateGrid(Actor* actor, Vector prevPos, Vector nextPos)
{
	// 이전의 위치가 변경되었는지 확인하여 그리드 갱신
	Cell prevCell = Cell::ConvertToCell(prevPos, BLOCK_SIZE);
	Cell currCell = Cell::ConvertToCell(nextPos, BLOCK_SIZE);

	// 변경이 없으면 무시
	if (prevCell == currCell)
		return;

	// 이전 위치 정보 삭제
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

	// 만약 추가하려는 곳이 그리드 범위를 벗어나면 무시
	if (currCell.index_X < 0 || currCell.index_X >= _gridCountX || currCell.index_Y < 0 || currCell.index_Y >= _gridCountY)
		return;

	// 새로운 위치 추가
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

	_actors.emplace(actor);
	_renderList[actor->GetRenderLayer()].emplace_back(actor);
}

void Scene::removeActor(Actor* actor)
{
	if (nullptr == actor)
		return;

	if (actor->GetRenderLayer() < 0 || actor->GetRenderLayer() >= RenderLayer::RL_Count)
		return;

	// 렌더 리스트에서 삭제
	{
		auto& list = _renderList[actor->GetRenderLayer()];
		auto iter = std::find(list.begin(), list.end(), actor);
		if (iter != list.end())
		{
			list.erase(iter);
		}
	}

	// 그리드에서 삭제
	Cell prevCell = Cell::ConvertToCell(actor->GetPos(), BLOCK_SIZE);
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

	// 전체 actor set 에서 제거
	{
		auto iter = _actors.find(actor);
		if (iter != _actors.end())
		{
			_actors.erase(iter);
			delete actor;
		}
	}
}

void Scene::removeAllActor()
{
	for (int32 i = 0; i < RenderLayer::RL_Count; ++i)
	{
		_renderList[i].clear();
	}
	for (auto& grid : _grid)
	{
		grid.second._actors.clear();
	}

	for (auto actor : _reserveAdd)
	{
		delete actor;
	}
	_reserveAdd.clear();
	_reserveRemove.clear();

	for (auto iter : _actors)
	{
		delete iter;
	}
	_actors.clear();
}