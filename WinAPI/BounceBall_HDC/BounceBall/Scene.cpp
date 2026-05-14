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

	_ui.Init();
}

void Scene::Destory()
{
	_ui.Destroy();
	removeAllActor();
}

void Scene::Update(float deltaTime)
{
	for (auto actor : _actors)
	{
		actor->Update(deltaTime);
	}

	// �߰��� ���� ó��
	for (auto actor : _reserveAdd)
	{
		// ����� ���͸� ������ ����Ʈ�� �߰��Ѵ�.
		addActor(actor);
	}
	_reserveAdd.clear(); // �߰� �� ���� ����Ʈ �ʱ�ȭ

	// ������ ���� ó��
	for (Actor* actor : _reserveRemove)
	{
		removeActor(actor);
	}

	_reserveRemove.clear(); // ���� �� ���� ����Ʈ �ʱ�ȭ

	// ��ó�� �׼� ����
	for (auto& action : _postUpdateActions)
	{
		action();
	}
	_postUpdateActions.clear();

	_ui.Update();
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
	// ������ ��ġ�� ����Ǿ����� �׸��� ����
	Cell prevCell = Cell::ConvertToCell(prevPos, BLOCK_SIZE);
	Cell currCell = Cell::ConvertToCell(nextPos, BLOCK_SIZE);

	// ������ ���� �ʿ� ����
	if (prevCell == currCell)
		return;

	// ���� ������ ����
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

	// ���� �߰��Ϸ��� ���� ���� ���̸� ����
	if (currCell.index_X < 0 || currCell.index_X >= _gridCountX || currCell.index_Y < 0 || currCell.index_Y >= _gridCountY)
		return;

	// ���� ���� �߰�
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

	// ���� ����Ʈ���� ����
	{
		auto& list = _renderList[actor->GetRenderLayer()];
		auto iter = std::find(list.begin(), list.end(), actor);
		if (iter != list.end())
		{
			list.erase(iter);
		}
	}

	// �׸��忡�� ����
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

	// ���� actor set ���� ����
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