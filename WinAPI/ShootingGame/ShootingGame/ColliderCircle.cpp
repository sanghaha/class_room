#include "pch.h"
#include "ColliderCircle.h"
#include "Actor.h"

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
        // ������ �׷�����.
        HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0)); // ������ �� ����
        HBRUSH hBrush = (HBRUSH)GetStockObject(NULL_BRUSH); // ���� �귯�� ���

        HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
        HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

        // ���� �߽ɰ� ������ ����
        int32 left = (int32)(_centerPos.x - _radius);
        int32 top = (int32)(_centerPos.y - _radius);
        int32 right = (int32)(_centerPos.x + _radius);
        int32 bottom = (int32)(_centerPos.y + _radius);

        Ellipse(hdc, left, top, right, bottom); // �� �׸���

        // ���� GDI ��ü ����
        SelectObject(hdc, hOldPen);
        SelectObject(hdc, hOldBrush);

        // ������ GDI ��ü ����
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
    // ���� �߽ɰ� ������ ����
    Pos center;
    center.x = pos.x + (size.w * 0.5f);
    center.y = pos.y + (size.h * 0.5f);

    _owner = owner;
    _size = size;
    _centerPos = center;
    _radius = radius;
}
