#include "pch.h"
#include "ColliderAABB.h"
#include "Actor.h"
#include "Game.h"

bool ColliderAABB::drawDebug = true;

ColliderAABB::ColliderAABB(bool checkCell) : _checkCell(checkCell)
{
}

ColliderAABB::~ColliderAABB()
{
}

void ColliderAABB::Init(Actor* owner, Size size, Pos pos, Size rectSize)
{
    Pos center;
    center.x = pos.x + (size.w * 0.5f);
    center.y = pos.y + (size.h * 0.5f);

    _owner = owner;
    _size = size;
    _centerPos = center;
    _rectSize = rectSize;
}

bool ColliderAABB::CheckCollision(ColliderAABB* other)
{
    if (other == nullptr)
        return false;

    // AABB Collision check
    float left = _centerPos.x - _rectSize.w * 0.5f;
    float right = _centerPos.x + _rectSize.w * 0.5f;
    float top = _centerPos.y - _rectSize.h * 0.5f;
    float bottom = _centerPos.y + _rectSize.h * 0.5f;

    float oLeft = other->_centerPos.x - other->_rectSize.w * 0.5f;
    float oRight = other->_centerPos.x + other->_rectSize.w * 0.5f;
    float oTop = other->_centerPos.y - other->_rectSize.h * 0.5f;
    float oBottom = other->_centerPos.y + other->_rectSize.h * 0.5f;

    if (left < oRight && right > oLeft && top < oBottom && bottom > oTop)
    {
        return true;
    }

    return false;
}

void ColliderAABB::UpdateComponent(float deltaTime)
{
    if (_owner == nullptr)
        return;

    Pos center;
    center.x = _owner->GetPos().x + (_size.w * 0.5f);
    center.y = _owner->GetPos().y + (_size.h * 0.5f);

    _centerPos = center;
}

void ColliderAABB::RenderComponent(HDC hdc, Pos pos)
{
    if (drawDebug)
    {
        HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0)); // Green for AABB
        HBRUSH hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);

        HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
        HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

        Pos renderPos = Game::ConvertScreenPos(_centerPos);

        int32 left = (int32)(renderPos.x - _rectSize.w * 0.5f);
        int32 top = (int32)(renderPos.y - _rectSize.h * 0.5f);
        int32 right = (int32)(renderPos.x + _rectSize.w * 0.5f);
        int32 bottom = (int32)(renderPos.y + _rectSize.h * 0.5f);

        ::Rectangle(hdc, left, top, right, bottom);

        SelectObject(hdc, hOldPen);
        SelectObject(hdc, hOldBrush);
        DeleteObject(hPen);
    }
}
