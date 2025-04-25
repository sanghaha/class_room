#include "pch.h"
#include "CollisionManager.h"
#include "Scene.h"
#include "ColliderCircle.h"
#include "Actor.h"
#include "Game.h"

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

void CollisionManager::checkCellCollision(Actor* actor, const Cell& cell)
{
    if (nullptr == actor)
        return;

    // 9���� ������ ���� �浹üũ
    for (int32 i = -1; i < 2; ++i)
    {
        for (int32 j = -1; j < 2; ++j)
        {
            Cell checkCell{ cell.index_X + i, cell.index_Y + j };
            const GridInfo& gridInfo = Game::GetScene()->GetGridInfo(checkCell);
            for (const auto& other : gridInfo._actors)
            {
                if (actor->GetCollider()->CheckCollision(other->GetCollider()))
                {
                    addCollisionState(actor->GetCollider(), other->GetCollider());
                }
                else
                {
                    removeCollisionState(actor->GetCollider(), other->GetCollider());
                }
            }
        }
    }
}
