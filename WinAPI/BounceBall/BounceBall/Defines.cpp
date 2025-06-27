#include "pch.h"
#include "Defines.h"

void PrintLog(wstring log)
{
	OutputDebugStringW((log + L"\n").c_str());
}

int32 RandRange(int32 min, int32 max)
{
    static std::random_device rd; // 하드웨어 엔트로피 소스
    static std::mt19937 gen(rd()); // Mersenne Twister 엔진

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
    // 사각형 안에서 원의 중심에 가장 가까운 점 계산
    float closestX = std::max(rect.left, std::min(circleX, rect.right));
    float closestY = std::max(rect.top, std::min(circleY, rect.bottom));

    // 원 중심에서 가까운 점까지 거리 계산
    float dx = circleX - closestX;
    float dy = circleY - closestY;

    // 거리의 제곱이 반지름 제곱보다 작거나 같으면 충돌
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
            // 법선 정규화
            outNormal = normal;

            // penetration 계산: radius - 실제 거리
            depth = radius - distance;
        }
        else
        {
            // 중심이 사각형 안에 있을 때: 임의 방향으로 밀어냄 (ex: 위쪽)
            outNormal = Vector(0, -1);
            depth = radius; // 완전히 겹친 상태
        }
    }
    return collision;
}

bool LineIntersectsAABB(Vector p0, Vector p1, const Rect& rect, Vector& outNormal, Vector& outPos)
{
    Vector dir = p1 - p0;

    float tMin = 0.0f;
    float tMax = 1.0f;
    Vector normal;

    // x축 검사
    if (dir.x != 0)
    {
        float tx1 = (rect.left - p0.x) / dir.x;
        float tx2 = (rect.right - p0.x) / dir.x;

        float t1 = min(tx1, tx2);
        float t2 = max(tx1, tx2);

        Vector nx = (tx1 < tx2) ? Vector(-1, 0) : Vector(1, 0);

        if (t1 > tMin) { tMin = t1; normal = nx; }
        if (t2 < tMax) tMax = t2;
    }
    else if (p0.x < rect.left || p0.x > rect.right)
    {
        return {}; // 평행하고 밖에 있음
    }

    // y축 검사
    if (dir.y != 0)
    {
        float ty1 = (rect.top - p0.y) / dir.y;
        float ty2 = (rect.bottom - p0.y) / dir.y;

        float t1 = min(ty1, ty2);
        float t2 = max(ty1, ty2);

        Vector ny = (ty1 < ty2) ? Vector(0, -1) : Vector(0, 1);

        if (t1 > tMin) { tMin = t1; normal = ny; }
        if (t2 < tMax) tMax = t2;
    }
    else if (p0.y < rect.top || p0.y > rect.bottom)
    {
        return {};
    }

    if (tMin > tMax || tMin > 1.0f || tMin < 0.0f)
        return {};

    // 충돌 결과 저장
    outPos = p0 + dir * tMin;
    outNormal = normal;

    return true;
}

bool IntersectSegment(
    Vector p1, Vector p2, // 선분 1
    Vector p3, Vector p4, // 선분 2
    Vector& outPos// 교차 좌표 결과 저장
)
{
    float dx1 = p2.x - p1.x;
    float dy1 = p2.y - p1.y;
    float dx2 = p4.x - p3.x;
    float dy2 = p4.y - p3.y;

    float denominator = dx1 * dy2 - dy1 * dx2;
    if (denominator == 0)
        return false; // 두선이 평행하다.

    float dx3 = p3.x - p1.x;
    float dy3 = p3.y - p1.y;

    float t = (dx3 * dy2 - dy3 * dx2) / denominator;
    float u = (dx3 * dy1 - dy3 * dx1) / denominator;

    if (t < 0 || t > 1 || u < 0 || u > 1)
        return false; // 선분 범위를 벗어남

    if (t == 0 && u == 0)
        return false; // 동일한 선이다

    // 교차점 계산
    outPos.x = p1.x + t * dx1;
    outPos.y = p1.y + t * dy1;
    return true;
}

bool IntersectSegmentRect(const Vector& A, const Vector& B, const Rect& r, Vector& outNormal, Vector& outPos)
{
    Vector TL = { r.left, r.top };
    Vector TR = { r.right, r.top };
    Vector BR = { r.right, r.bottom };
    Vector BL = { r.left, r.bottom };

    if (IntersectSegment(A, B, TL, TR, outPos))
    {
        outNormal = Vector(0, -1);
        return true;
    }
    if (IntersectSegment(A, B, TR, BR, outPos))
    {
        outNormal = Vector(1, 0);
        return true;
    }
    if (IntersectSegment(A, B, BR, BL, outPos))
    {
        outNormal = Vector(0, 1);
        return true;
    }
    if (IntersectSegment(A, B, BL, TL, outPos))
    {
        outNormal = Vector(-1, 0);
        return true;
    }

    outNormal = Vector(0, 0);
    outPos = Vector(0, 0);
    return false;
}