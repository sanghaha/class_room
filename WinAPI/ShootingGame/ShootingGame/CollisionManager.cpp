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
    // 충돌 검사가 필요한 객체들. 전체 리스트를 다 돌면서 비교해도 되지만. 필요한 정보들만 해보자
    // 총알 vs 비행기
    const auto& bulletList = Scene::GetInstance()->GetRenderList(RenderLayer::RL_Bullet);
    const vector<Actor*>& playerList = Scene::GetInstance()->GetRenderList(RenderLayer::RL_Player);
    const vector<Actor*>& enemyList = Scene::GetInstance()->GetRenderList(RenderLayer::RL_Enemy);

    // 내 비행기 + 적 비행기
    {
        vector<Actor*> airplaneList = enemyList;
        airplaneList.insert(airplaneList.end(), playerList.begin(), playerList.end()); // playerList 추가

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

    // 내 비행기 vs 적 비행기
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

    // src와 other를 묶어서 tuple 생성
    auto collisionPair = std::make_tuple(src, other);

    // 이미 충돌 상태인지 확인
    if (_collisionMap.find(collisionPair) == _collisionMap.end())
    {
        // 충돌 상태 추가
        _collisionMap.insert(collisionPair);

        // 충돌 시작 이벤트 호출
        src->OnEnterCollision(src, other);
        other->OnEnterCollision(other, src);
    }
    else
    {
        // 충돌 중 이벤트 호출
        src->OnOverlapCollision(src, other);
        other->OnOverlapCollision(other, src);
    }
}

void CollisionManager::removeCollisionState(ColliderCircle* src, ColliderCircle* other)
{
    if (src == nullptr || other == nullptr)
        return;

    // src와 other를 묶어서 tuple 생성
    auto collisionPair = std::make_tuple(src, other);

    // 충돌 상태에서 제거
    auto iter = _collisionMap.find(collisionPair);
    if (iter != _collisionMap.end())
    {
        _collisionMap.erase(iter);

        // 충돌 종료 이벤트 호출
        src->OnExitCollision(src, other);
        other->OnExitCollision(other, src);
    }
}
