#include "pch.h"
#include "Scene.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "Sprite.h"
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

	_ui.Init();
}

void Scene::Destory()
{
	_ui.Destroy();
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


	_ui.Update();
}

void Scene::Render(ID2D1RenderTarget* renderTarget)
{
	for (auto list : _renderList)
	{
		for (auto actor : list)
		{
			actor->Render(renderTarget);
		}
	}

	_ui.Render(renderTarget);
}

void Scene::ReserveRemove(Actor* actor)
{
	if (nullptr == actor)
		return;

	if (_reserveRemove.contains(actor))
		return;

	_reserveRemove.emplace(actor);
}

void Scene::UpdateGrid(Actor* actor, Vector prevPos, Vector nextPos)
{
	// 액터의 위치가 변경되었으니 그리드 갱신
	Cell prevCell = Cell::ConvertToCell(prevPos, BLOCK_SIZE);
	Cell currCell = Cell::ConvertToCell(nextPos, BLOCK_SIZE);

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
}

void Scene::removeActor(Actor* actor)
{
	if (nullptr == actor)
		return;

	if (actor->GetRenderLayer() < 0 || actor->GetRenderLayer() >= RenderLayer::RL_Count)
		return;

	// 렌더 리스트에서 제거
	{
		auto& list = _renderList[actor->GetRenderLayer()];
		auto iter = std::find(list.begin(), list.end(), actor);
		if (iter != list.end())
		{
			list.erase(iter);
		}
	}

	// 그리드에서 제거
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

	_reserveAdd.clear();
	_reserveRemove.clear();

	for(auto iter : _actors)
	{
		delete iter;
	}
	_actors.clear();
}