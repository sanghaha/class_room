#include "pch.h"
#include "ColliderCircle.h"
#include "Actor.h"
#include "Game.h"

bool ColliderCircle::drawDebug = true;

ColliderCircle::ColliderCircle(bool checkCell) : _checkCell(checkCell)
{
}

ColliderCircle::~ColliderCircle()
{
}

bool ColliderCircle::CheckCollision(ColliderCircle* other)
{
    if (other == nullptr)
        return false;

    float dx = GetCenterPos().x - other->GetCenterPos().x;
    float dy = GetCenterPos().y - other->GetCenterPos().y;
    float distance = sqrt(dx * dx + dy * dy);

    if (distance < GetRadius() + other->GetRadius())
    {
        return true;
    }
    return false;
}

void ColliderCircle::Render(HDC hdc, Pos pos)
{
    if (drawDebug)
    {
        // 원형을 그려보자.
        HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0)); // 빨간색 펜 생성
        HBRUSH hBrush = (HBRUSH)GetStockObject(NULL_BRUSH); // 투명 브러시 사용

        HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
        HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

        Pos renderPos = Game::ConvertScreenPos(_centerPos);

        // 원의 중심과 반지름 설정
        int32 left = (int32)(renderPos.x - _radius);
        int32 top = (int32)(renderPos.y - _radius);
        int32 right = (int32)(renderPos.x + _radius);
        int32 bottom = (int32)(renderPos.y + _radius);

        Ellipse(hdc, left, top, right, bottom); // 원 그리기

        // 이전 GDI 객체 복원
        SelectObject(hdc, hOldPen);
        SelectObject(hdc, hOldBrush);

        // 생성한 GDI 객체 삭제
        DeleteObject(hPen);
    }
}

void ColliderCircle::Update(float deltaTime)
{
	if (_owner == nullptr)
		return;

    Pos center;
    center.x = _owner->GetPos().x + (_size.w * 0.5f);
    center.y = _owner->GetPos().y + (_size.h * 0.5f);

    _centerPos = center;
}

void ColliderCircle::Init(Actor* owner, Size size, Pos pos, int32 radius)
{
    // 원의 중심과 반지름 설정
    Pos center;
    center.x = pos.x + (size.w * 0.5f);
    center.y = pos.y + (size.h * 0.5f);

    _owner = owner;
    _size = size;
    _centerPos = center;
    _radius = radius;
}
