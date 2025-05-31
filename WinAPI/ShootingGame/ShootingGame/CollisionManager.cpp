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
    // 충돌 검사가 필요한 객체들. 전체 리스트를 다 돌면서 비교해도 되지만. 필요한 정보들만 해보자
     
    // 내 비행기 + 내 비행기의 총알
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

        // 펜 생성
        HPEN myPen = CreatePen(PS_SOLID, 3, RGB(0, 255, 255));
        HPEN oldPen = (HPEN)SelectObject(hdc, myPen);

        // 충돌 체크가 필요한 선만 그리기
        for (auto iter : _checkCollider)
        {
            const Cell& cell = Game::GetScene()->GetCell(iter->GetCenterPos());

            // 인접한 셀 모두 표시
            for (int32 i = -1; i < 2; ++i)
            {
                for (int32 j = -1; j < 2; ++j)
                {
                    Cell checkCell{ cell.index_X + i, cell.index_Y + j };


                    // 사각형 그리기
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

        // 이전 펜 복원 및 새 펜 삭제
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

void CollisionManager::checkCellCollision(ColliderCircle* collider, const Cell& cell)
{
    if (nullptr == collider)
        return;

    // Bullet이 있고, 인접한 비행기만 충돌체크
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
