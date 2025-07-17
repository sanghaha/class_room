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

bool CheckCircleAABB(float circleX, float circleY, float radius, Rect rect, Vector& outNormal, Vector& outPos)
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
    float depth = 0;

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
			outPos = Vector(closestX, closestY);
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

bool LineIntersectsAABB(Vector p0, Vector p1, const Rect& rect, Vector& outNormal, Vector& outPos)
{
    Vector dir = p1 - p0;

    float tMin = 0.0f;
    float tMax = 1.0f;
    Vector normal;

    // x�� �˻�
    if (dir.x != 0)
    {
        float tx1 = (rect.left - p0.x) / dir.x;
        float tx2 = (rect.right - p0.x) / dir.x;

        Vector nx = (tx1 < tx2) ? Vector(-1, 0) : Vector(1, 0);
        if (tx1 > tx2) std::swap(tx1, tx2);

        if (tx1 > tMin)
        {
            normal = nx;
        }

        tMin = std::max(tMin, tx1);
        tMax = std::min(tMax, tx2);

        if (tMin > tMax)
            return false; // �浹 �� ��
    }
    else if (p0.x < rect.left || p0.x > rect.right)
    {
        return false; // Y������ �����ϰ� �ۿ� ����
    }

    // y�� �˻�
    if (dir.y != 0)
    {
        float ty1 = (rect.top - p0.y) / dir.y;
        float ty2 = (rect.bottom - p0.y) / dir.y;

        // ���� �������κ��� �������ִ� �Ÿ� ����
        // ty1 �� ty2���� �۴ٸ�, ������ �Ʒ� �����̴�.
        Vector ny = (ty1 < ty2) ? Vector(0, -1) : Vector(0, 1);

        if (ty1 > ty2) std::swap(ty1, ty2);

        if (ty1 > tMin)
        {
            normal = ny;
        }

        tMin = std::max(tMin, ty1);
        tMax = std::min(tMax, ty2);

        if (tMin > tMax)
            return false; // �浹 �� ��
    }
    else if (p0.y < rect.top || p0.y > rect.bottom)
    {
        return false;
    }

    // ������ �簢�� ���ο� �ִ� ��� ����ó��
    // ������ ������⿡ ���� �� �������� ���ݴ� ������ �������� normal �� �������ش�.
    if (normal == Vector(0, 0))
    {
        if (fabs(dir.x) > fabs(dir.y))
        {
            normal = (dir.x > 0) ? Vector(-1, 0) : Vector(1, 0);
        }
        else
        {
            normal = (dir.y > 0) ? Vector(0, -1) : Vector(0, 1);
        }
    }

    //if (tMin > tMax || tMin > 1.0f || tMin < 0.0f)
    //    return false;

    // �浹 ��� ����
    outPos = p0 + dir * tMin;
    outNormal = normal;

    return true;
}

bool IntersectSegment(
    Vector p1, Vector p2, // ���� 1
    Vector p3, Vector p4, // ���� 2
    Vector& outPos// ���� ��ǥ ��� ����
)
{
    float dx1 = p2.x - p1.x;
    float dy1 = p2.y - p1.y;
    float dx2 = p4.x - p3.x;
    float dy2 = p4.y - p3.y;

    float denominator = dx1 * dy2 - dy1 * dx2;
    if (denominator < 1e-6f)
        return false; // �μ��� �����ϴ�.

    float dx3 = p3.x - p1.x;
    float dy3 = p3.y - p1.y;

    float t = (dx3 * dy2 - dy3 * dx2) / denominator;
    float u = (dx3 * dy1 - dy3 * dx1) / denominator;

    if (t < 0 || t > 1 || u < 0 || u > 1)
        return false; // ���� ������ ���

    if (t == 0 && u == 0)
        return false; // ������ ���̴�

    // ������ ���
    outPos.x = p1.x + t * dx1;
    outPos.y = p1.y + t * dy1;
    return true;
}

bool IntersectSegmentSegment(const Vector& A, const Vector& B, const Vector& C, const Vector& D, Vector& outIntersection)
{
    Vector r = B - A;
    Vector s = D - C;
    Vector AC = C - A;

    float rxs = r.Cross(s);
    float ACxs = AC.Cross(s);
    float ACxr = AC.Cross(r);

    const float EPSILON = 1e-6f;

    if (fabs(rxs) < EPSILON) {
        // r x s == 0 �� �����ϰų� ��ġ
        return false;
    }

    float t = ACxs / rxs;
    float u = ACxr / rxs;

    if (t >= 0 && t <= 1 && u >= 0 && u <= 1)
    {
        outIntersection = Vector(A.x + t * r.x, A.y + t * r.y);
        return true;
    }

    return false;
}

bool IntersectSegmentRect(const Vector& A, const Vector& B, const Rect& r, Vector& outNormal, Vector& outPos)
{
    struct HitResult 
    {
        bool hit = false;
        Vector normal;
        Vector pos;
        float distSq = FLT_MAX;
    };

    HitResult result;
    Vector TL = { r.left, r.top };
    Vector TR = { r.right, r.top };
    Vector BR = { r.right, r.bottom };
    Vector BL = { r.left, r.bottom };

    auto tryHit = [&](Vector p1, Vector p2, Vector normal) 
		{
			Vector pos;
			//if (IntersectSegment(A, B, p1, p2, pos)) 
            if(IntersectSegmentSegment(A,B,p1,p2,pos))
            {
				float distSq = (A - pos).LengthSquared();

                // ���� ����� ������ ã��
				if (distSq < result.distSq) {
					result.hit = true;
					result.normal = normal;
					result.pos = pos;
					result.distSq = distSq;
				}
			}
		};

    tryHit(TL, TR, { 0, -1 });
    tryHit(TR, BR, { 1, 0 });
    tryHit(BR, BL, { 0, 1 });
    tryHit(BL, TL, { -1, 0 });

    //if (IntersectSegment(A, B, TL, TR, outPos))
    //{
    //    outNormal = Vector(0, -1);
    //}
    //if (IntersectSegment(A, B, TR, BR, outPos))
    //{
    //    outNormal = Vector(1, 0);
    //}
    //if (IntersectSegment(A, B, BR, BL, outPos))
    //{
    //    outNormal = Vector(0, 1);
    //}
    //if (IntersectSegment(A, B, BL, TL, outPos))
    //{
    //    outNormal = Vector(-1, 0);
    //}

    if (result.hit) 
    {
        outNormal = result.normal;
        outPos = result.pos;
        return true;
    }

    outNormal = Vector(0, 0);
    outPos = Vector(0, 0);
    return false;
}