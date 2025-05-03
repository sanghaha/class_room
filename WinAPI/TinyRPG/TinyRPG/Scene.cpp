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
	// ���ҽ� �ε�
	loadResources();

	// ������Ʈ ����
	createObjects();

	// Ÿ�̸� �߰�
	initTimer();

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

	// �׸��� ����׿�
	if (_drawGridCell)
	{
		drawGrid(renderTarget);
	}
}

void Scene::drawGrid(ID2D1HwndRenderTarget* renderTarget)
{
	// ȭ�� ũ��� �׸��� ũ�� ����
	int32 width = _gridCountX * GTileSize;
	int32 height = _gridCountY * GTileSize;

	auto brush = ResourceManager::GetInstance()->GetBrush(BrushColor::Red);

	// ���μ� �׸���
	for (int y = 0; y <= height; y += GTileSize)
	{
		Vector renderPos1 = Game::ConvertRenderPos(Vector((float)0, (float)y));
		Vector renderPos2 = Game::ConvertRenderPos(Vector((float)width, (float)y));
		
		D2D1_POINT_2F start = D2D1::Point2F(renderPos1.x, renderPos1.y);
		D2D1_POINT_2F end = D2D1::Point2F(renderPos2.x, renderPos2.y);
		renderTarget->DrawLine(start, end, brush, 1.0f);
	}

	// ���μ� �׸���
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
	// ������ ��ġ�� ����Ǿ����� �׸��� ����
	// ������ ���� �ʿ� ����
	if (prevCell == currCell)
		return;

	// ���� ������ ����
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

				// ������ �ִ� actor��� �浹 �����ٰ� �˷��ش�.
				for (auto iter : gridInfo._actorsInCell)
				{
					iter->OnEndOverlapActor(actor);
					actor->OnEndOverlapActor(iter);
				}
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
			if (false == gridInfo._actorsInCell.contains(actor))
			{
				// ó�� �����ϴ� ���̴ϱ�, ������ �ִ� actor��� �浹�Ǿ��ٰ� �˷��ش�.
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
	// �ش� ��ġ�� ī�޶� ������ Ȯ��
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

	// ���� ����Ʈ���� ����
	{
		auto& list = _renderList[actor->GetRenderLayer()];
		auto iter = std::find(list.begin(), list.end(), actor);
		if (iter != list.end())
		{
			list.erase(iter);
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
