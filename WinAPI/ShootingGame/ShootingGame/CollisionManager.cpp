#include "pch.h"
#include "CollisionManager.h"
#include "Scene.h"
#include "GameScene.h"
#include "ColliderCircle.h"
#include "Actor.h"
#include "Game.h"
#include "Player.h"

CollisionManager::CollisionManager()
{
}

CollisionManager::~CollisionManager()
{
}

void CollisionManager::Init()
{
}

void CollisionManager::Update()
{
    // �浹 �˻簡 �ʿ��� ��ü��. ��ü ����Ʈ�� �� ���鼭 ���ص� ������. �ʿ��� �����鸸 �غ���
     
    // �� ����� + �� ������� �Ѿ�
    for (auto iter : _checkCollider)
    {
        const Cell& cell = Game::GetScene()->GetCell(iter->GetCenterPos());
        checkCellCollision(iter, cell);
    }
}

void CollisionManager::Render(HDC hdc)
{
    if (Scene::drawDebugCell)
    {
        int32 gridSize = Game::GetInstance()->GetScene()->GetGridSize();

        // �� ����
        HPEN myPen = CreatePen(PS_SOLID, 3, RGB(0, 255, 255));
        HPEN oldPen = (HPEN)SelectObject(hdc, myPen);

        // �浹 üũ�� �ʿ��� ���� �׸���
        for (auto iter : _checkCollider)
        {
            const Cell& cell = Game::GetScene()->GetCell(iter->GetCenterPos());

            // ������ �� ��� ǥ��
            for (int32 i = -1; i < 2; ++i)
            {
                for (int32 j = -1; j < 2; ++j)
                {
                    Cell checkCell{ cell.index_X + i, cell.index_Y + j };


                    // �簢�� �׸���
                    int32 x = checkCell.index_X * gridSize;
                    int32 y = checkCell.index_Y * gridSize;

                    {
                        MoveToEx(hdc, x, y, nullptr);
                        LineTo(hdc, x + gridSize, y);
                    }
                    {
                        MoveToEx(hdc, x + gridSize, y, nullptr);
                        LineTo(hdc, x + gridSize, y + gridSize);
                    }
                    {
                        MoveToEx(hdc, x + gridSize, y + gridSize, nullptr);
                        LineTo(hdc, x, y + gridSize);
                    }
                    {
                        MoveToEx(hdc, x, y + gridSize, nullptr);
                        LineTo(hdc, x, y);
                    }
                }
            }
        }

        // ���� �� ���� �� �� �� ����
        SelectObject(hdc, oldPen);
        DeleteObject(myPen);
    }
}

void CollisionManager::AddCheckCollider(ColliderCircle* collider)
{
    _checkCollider.insert(collider);
}

void CollisionManager::RemoveCheckCollider(ColliderCircle* collider)
{
    _checkCollider.erase(collider);
}

void CollisionManager::addCollisionState(ColliderCircle* src, ColliderCircle* other)
{
    if (src == nullptr || other == nullptr)
        return;

    // src�� other�� ��� tuple ����
    auto collisionPair = std::make_tuple(src, other);

    // �̹� �浹 �������� Ȯ��
    if (_collisionMap.find(collisionPair) == _collisionMap.end())
    {
        // �浹 ���� �߰�
        _collisionMap.insert(collisionPair);

        // �浹 ���� �̺�Ʈ ȣ��
        src->OnEnterCollision(src, other);
        other->OnEnterCollision(other, src);
    }
    else
    {
        // �浹 �� �̺�Ʈ ȣ��
        src->OnOverlapCollision(src, other);
        other->OnOverlapCollision(other, src);
    }
}

void CollisionManager::removeCollisionState(ColliderCircle* src, ColliderCircle* other)
{
    if (src == nullptr || other == nullptr)
        return;

    // src�� other�� ��� tuple ����
    auto collisionPair = std::make_tuple(src, other);

    // �浹 ���¿��� ����
    auto iter = _collisionMap.find(collisionPair);
    if (iter != _collisionMap.end())
    {
        _collisionMap.erase(iter);

        // �浹 ���� �̺�Ʈ ȣ��
        src->OnExitCollision(src, other);
        other->OnExitCollision(other, src);
    }
}

void CollisionManager::checkCellCollision(ColliderCircle* collider, const Cell& cell)
{
    if (nullptr == collider)
        return;

    // Bullet�� �ְ�, ������ ����⸸ �浹üũ
    for (int32 i = -1; i < 2; ++i)
    {
        for (int32 j = -1; j < 2; ++j)
        {
            Cell checkCell{ cell.index_X + i, cell.index_Y + j };
            const GridInfo& gridInfo = Game::GetScene()->GetGridInfo(checkCell);
            for (const auto& other : gridInfo._actors)
            {
                if (collider->CheckCollision(other->GetCollider()))
                {
                    addCollisionState(collider, other->GetCollider());
                }
                else
                {
                    removeCollisionState(collider, other->GetCollider());
                }
            }
        }
    }
}
