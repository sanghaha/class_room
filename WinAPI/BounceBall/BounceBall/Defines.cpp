#include "pch.h"
#include "Defines.h"

void PrintLog(wstring log)
{
	OutputDebugStringW((log + L"\n").c_str());
}

int32 RandRange(int32 min, int32 max)
{
    static std::random_device rd; // �ϵ���� ��Ʈ���� �ҽ�
    static std::mt19937 gen(rd()); // Mersenne Twister ����

    std::uniform_int_distribution<> dist(min, max);
    return dist(gen);
}

bool IsInPoint(RECT rect, POINT pos)
{
    if (pos.x < rect.left)
        return false;

    if (pos.x > rect.right)
        return false;

    if (pos.y < rect.top)
        return false;

    if (pos.y > rect.bottom)
        return false;
    return true;
}

bool CheckCircleAABB(float circleX, float circleY, float radius, Rect rect, Vector& outNormal, float& depth)
{
    // �簢�� �ȿ��� ���� �߽ɿ� ���� ����� �� ���
    float closestX = std::max(rect.left, std::min(circleX, rect.right));
    float closestY = std::max(rect.top, std::min(circleY, rect.bottom));

    // �� �߽ɿ��� ����� ������ �Ÿ� ���
    float dx = circleX - closestX;
    float dy = circleY - closestY;

    // �Ÿ��� ������ ������ �������� �۰ų� ������ �浹
    float distanceSq = dx * dx + dy * dy;
    float radiusSq = radius * radius;

    bool collision = (distanceSq <= radiusSq);
    if (collision)
    {
        Vector normal = Vector(circleX, circleY) - Vector(closestX, closestY);
        normal.Normalize();

        OutputDebugStringW(std::format(L"@ Collision : {0}, {1}\n", closestX, closestY).c_str());

        float distance = sqrt(distanceSq);
        if (distance != 0)
        {
            // ���� ����ȭ
            outNormal = normal;

            // penetration ���: radius - ���� �Ÿ�
            depth = radius - distance;
        }
        else
        {
            // �߽��� �簢�� �ȿ� ���� ��: ���� �������� �о (ex: ����)
            outNormal = Vector(0, -1);
            depth = radius; // ������ ��ģ ����
        }
    }
    return collision;
}

bool LineIntersectsAABB(Vector p0, Vector p1, const Rect& rect, Vector& outNormal, float& tHit)
{
    Vector dir = p1 - p0;
    float tMin = 0.0f;
    float tMax = 1.0f;
    Vector normalCandidate; // tMin�� ���� ���� normal �ĺ�

    for (int axis = 0; axis < 2; ++axis)
    {
        float origin = axis == 0 ? p0.x : p0.y;
        float direction = axis == 0 ? dir.x : dir.y;
        float minBound = axis == 0 ? rect.left : rect.top;
        float maxBound = axis == 0 ? rect.right : rect.bottom;

        if (fabs(direction) < 1e-6f)
        {
            // ������ �����ѵ� AABB ���̸� �浹 X
            if (origin < minBound || origin > maxBound)
                return false;

            // AABB ���� �鿡 �پ� �ִ� ��� �� normal ����
            if (fabs(origin - minBound) < 1.0f)
                normalCandidate = (axis == 0) ? Vector(-1, 0) : Vector(0, -1);
            else if (fabs(origin - maxBound) < 1.0f)
                normalCandidate = (axis == 0) ? Vector(1, 0) : Vector(0, 1);
        }
        else
        {
            float t1 = (minBound - origin) / direction;
            float t2 = (maxBound - origin) / direction;

            Vector normal1 = Vector(0, 0);
            Vector normal2 = Vector(0, 0);
            if (axis == 0)
            {
                normal1 = Vector(-1, 0); // ���� ����
                normal2 = Vector(1, 0);  // ������ ����
            }
            else
            {
                normal1 = Vector(0, -1); // ���� ����
                normal2 = Vector(0, 1);  // �Ʒ��� ����
            }

            if (t1 > t2)
            {
                std::swap(t1, t2);
                std::swap(normal1, normal2); // ���⵵ �Բ� ����
            }

            if (t1 > tMin)
            {
                tMin = t1;
                normalCandidate = normal1; // ���� ���� �ʰ� ������ normal ���
            }

            tMax = std::min(tMax, t2);

            if (tMin > tMax)
                return false;
        }
    }

    outNormal = normalCandidate;
    tHit = tMin;
    return true;
}