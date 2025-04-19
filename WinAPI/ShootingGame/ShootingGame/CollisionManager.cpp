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
            const Cell& cell = Scene::GetInstance()->GetCell(bullet->GetPos());
           
            // Bullet이 있고, 인접한 비행기만 충돌체크
            checkCellCollision(bullet, cell);
        }
    }

    // 내 비행기 vs 적 비행기
    for (const auto& player : playerList)
    {
        if(player->GetCollider() == nullptr)
            continue;

        const Cell& cell = Scene::GetInstance()->GetCell(player->GetPos());
        checkCellCollision(player, cell);
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

void CollisionManager::checkCellCollision(Actor* actor, const Cell& cell)
{
    if (nullptr == actor)
        return;

    // Bullet이 있고, 인접한 비행기만 충돌체크
    for (int32 i = -1; i < 2; ++i)
    {
        for (int32 j = -1; j < 2; ++j)
        {
            Cell checkCell{ cell.index_X + i, cell.index_Y + j };
            const GridInfo& gridInfo = Scene::GetInstance()->GetGridInfo(checkCell);
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
