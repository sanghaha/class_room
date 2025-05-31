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
	// ���ҽ� �ε�
	loadResources();

	// ������Ʈ ����
	createObjects();

	// Ÿ�̸� �߰�
	initTimer();

	// �׸��� ����
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
	
	// �׸��� ����׿�
	if (drawDebugCell)
	{
		drawGrid(hdc);
	}
}

void Scene::drawGrid(HDC hdc)
{
	// ȭ�� ũ��� �׸��� ũ�� ����
	int32 width = GWinSizeX;
	int32 height = GWinSizeY;

	// ������ �� ����
	HPEN redPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	HPEN oldPen = (HPEN)SelectObject(hdc, redPen);

	// ���μ� �׸���
	for (int y = 0; y <= height; y += _gridSize)
	{
		MoveToEx(hdc, 0, y, nullptr); // ������ ����
		LineTo(hdc, width, y);        // �������� �� �׸���
	}

	// ���μ� �׸���
	for (int x = 0; x <= width; x += _gridSize)
	{
		MoveToEx(hdc, x, 0, nullptr); // ������ ����
		LineTo(hdc, x, height);       // �������� �� �׸���
	}

	// ���� �� ���� �� �� �� ����
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
	// ������ ��ġ�� ����Ǿ����� �׸��� ����
	Cell prevCell = Cell::ConvertToCell(prevPos, _gridSize);
	Cell currCell = Cell::ConvertToCell(nextPos, _gridSize);

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

	actor->Init();
	_actors.emplace(actor);
	_renderList[actor->GetRenderLayer()].emplace_back(actor);

	// �浹üũ ���
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

	// ���� ����Ʈ���� ����
	{
		auto& list = _renderList[actor->GetRenderLayer()];
		auto iter = std::find(list.begin(), list.end(), actor);
		if (iter != list.end())
		{
			list.erase(iter);
		}
	}

	// �浹üũ ����
	if (actor->GetCollider() && actor->GetCollider()->CheckCell())
	{
		CollisionManager::GetInstance()->RemoveCheckCollider(actor->GetCollider());
	}

	// �׸��忡�� ����
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
