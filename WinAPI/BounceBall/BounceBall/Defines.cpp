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

bool LineIntersectsAABB(Vector p0, Vector p1, const Rect& rect, Vector& outNormal, float& tHit)
{
    Vector dir = p1 - p0;
    float tMin = 0.0f;
    float tMax = 1.0f;
    Vector normalCandidate; // tMin을 만든 축의 normal 후보

    for (int axis = 0; axis < 2; ++axis)
    {
        float origin = axis == 0 ? p0.x : p0.y;
        float direction = axis == 0 ? dir.x : dir.y;
        float minBound = axis == 0 ? rect.left : rect.top;
        float maxBound = axis == 0 ? rect.right : rect.bottom;

        if (fabs(direction) < 1e-6f)
        {
            // 선분이 평행한데 AABB 밖이면 충돌 X
            if (origin < minBound || origin > maxBound)
                return false;

            // AABB 슬랩 면에 붙어 있는 경우 → normal 결정
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
                normal1 = Vector(-1, 0); // 왼쪽 슬랩
                normal2 = Vector(1, 0);  // 오른쪽 슬랩
            }
            else
            {
                normal1 = Vector(0, -1); // 위쪽 슬랩
                normal2 = Vector(0, 1);  // 아래쪽 슬랩
            }

            if (t1 > t2)
            {
                std::swap(t1, t2);
                std::swap(normal1, normal2); // 방향도 함께 스왑
            }

            if (t1 > tMin)
            {
                tMin = t1;
                normalCandidate = normal1; // 현재 가장 늦게 진입한 normal 기록
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