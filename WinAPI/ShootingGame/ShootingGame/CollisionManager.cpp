#include "pch.h"
#include "CollisionManager.h"
#include "Scene.h"
#include "ColliderCircle.h"
#include "Actor.h"

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
    // �Ѿ� vs �����
    const auto& bulletList = Scene::GetInstance()->GetRenderList(RenderLayer::RL_Bullet);
    const vector<Actor*>& playerList = Scene::GetInstance()->GetRenderList(RenderLayer::RL_Player);
    const vector<Actor*>& enemyList = Scene::GetInstance()->GetRenderList(RenderLayer::RL_Enemy);

    // �� ����� + �� �����
    {
        vector<Actor*> airplaneList = enemyList;
        airplaneList.insert(airplaneList.end(), playerList.begin(), playerList.end()); // playerList �߰�

        for (const auto& bullet : bulletList)
        {
            if (bullet->GetCollider())
            {
                for (const auto& airplane : airplaneList)
                {
                    if (bullet->GetCollider()->CheckCollision(airplane->GetCollider()))
                    {
                        addCollisionState(bullet->GetCollider(), airplane->GetCollider());
                    }
                    else
                    {
                        removeCollisionState(bullet->GetCollider(), airplane->GetCollider());
                    }
                }
            }
        }
    }

    // �� ����� vs �� �����
    for (const auto& player : playerList)
    {
        if(player->GetCollider() == nullptr)
            continue;

        for (const auto& enemy : enemyList)
        {
            if (player->GetCollider()->CheckCollision(enemy->GetCollider()))
            {
                addCollisionState(player->GetCollider(), enemy->GetCollider());
            }
            else
            {
                removeCollisionState(player->GetCollider(), enemy->GetCollider());
            }
        }
    }
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
