#include "pch.h"
#include "Ball.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "Sprite.h"
#include "Game.h"
#include "GameScene.h"
#include "Star.h"

Ball::Ball(Vector pos) : Super(pos)
{
}

Ball::~Ball()
{
    _sprite = nullptr;
}

void Ball::Init()
{
    Super::Init();

    _sprite = CreateSpriteComponent("Ball_0", BALL_SIZE, BALL_SIZE);
}

void Ball::Update(float deltaTime)
{
    Super::Update(deltaTime);

    acceleration = Vector(0, 0);
    float moveForce = 500.0f;        // 힘의 크기
    float mass = 1.0f;               // 질량 (고정값)
    float frictionCoef = 10.0f;

    if (InputManager::GetInstance()->GetButtonPressed(KeyType::Left))
    {
        acceleration.x -= moveForce / mass;
        OutputDebugStringW(std::format(L"Left Key Pressed\n").c_str());
    }
    if (InputManager::GetInstance()->GetButtonPressed(KeyType::Right))
    {
        acceleration.x += moveForce / mass;
        OutputDebugStringW(std::format(L"Right Key Pressed\n").c_str());;
    }

    applyPhysics(deltaTime);
}

void Ball::applyPhysics(float deltaTime)
{
    GameScene* scene = dynamic_cast<GameScene*>(Game::GetInstance()->GetScene());
    if (nullptr == scene)
        return;

    // 물리 효과 적용
    // 속도에 중력 적용
    acceleration.y += gravity;

    // 속도에 가속도 적용
    velocity += acceleration * deltaTime;

    float upFactor = 330;	// 양옆이동
    if (velocity.y > upFactor)
        velocity.y = upFactor;
    if (velocity.y < -upFactor)
        velocity.y = -upFactor;

    float sideFactor = 150;	// 양옆이동
    if (velocity.x > sideFactor)
        velocity.x = sideFactor;
    if (velocity.x < -sideFactor)
        velocity.x = -sideFactor;

    // 좌우 마찰력
    velocity.x *= 0.95f;
    if (std::abs(velocity.x) < 1.0f) // 임계값
        velocity.x = 0.0f;

    // 위치 갱신
    Vector newPos = GetPos() + velocity * deltaTime;

    OutputDebugStringW(std::format(L"vel:{0},{1}, acc:{2},{3}, prevPos:{4},{5} newPos:{6},{7}\n", velocity.x, velocity.y, acceleration.x, acceleration.y, GetPos().x, GetPos().y, newPos.x, newPos.y).c_str());

    // 바닥 충돌 체크
    Vector dir = newPos - GetPos();
    dir.Normalize();

    Vector normal;
    float tHit = 0;
    if (scene->CheckCollision(this, GetPos(), newPos, normal, tHit))
    {
        Vector hitPos = GetPos() + dir * tHit;

        // 반사 벡터 계산
        Vector reflect = velocity - normal * (2.0f * velocity.Dot(normal));
        _debug_reflect = reflect;
        _debug_reflect.Normalize();

        // 좌우 벽에 부딪힌 상태면 그냥 x축 보정만 해준다.
        if (normal.x != 0)
        {
            velocity.x = reflect.x;
        }
        
        if(normal.y < 0)
        {
            velocity.y = -upFactor;
        }

        const float epsilon = 5.0f;
        newPos = hitPos + (normal * epsilon);

        //velocity = reflect;
        OutputDebugStringW(std::format(L"[Collision] vel:{0},{1}, tHit:{2}, normal:{3},{4}\n", velocity.x, velocity.y, tHit, normal.x, normal.y).c_str());

        _debug_prePos = GetPos();
        _debug_normal = normal;
        _debug_newPos = newPos;
    }

    SetPos(newPos);
}


void Ball::Render(ID2D1RenderTarget* renderTarget)
{
    Super::Render(renderTarget);

    auto brushRed = ResourceManager::GetInstance()->GetBrush(BrushColor::Red);
    auto brushBlue = ResourceManager::GetInstance()->GetBrush(BrushColor::Blue);
    auto brushGreen = ResourceManager::GetInstance()->GetBrush(BrushColor::Green);

    {
        D2D1_ELLIPSE dot = D2D1::Ellipse(D2D1_POINT_2F(_debug_prePos.x, _debug_prePos.y), 3, 3);
        renderTarget->FillEllipse(dot, brushRed);
    }
    {
        D2D1_ELLIPSE dot = D2D1::Ellipse(D2D1_POINT_2F(_debug_newPos.x, _debug_newPos.y), 3, 3);
        renderTarget->FillEllipse(dot, brushBlue);
    }
    {
        D2D1_POINT_2F pos(_debug_prePos.x, _debug_prePos.y);
        Vector endPos = _debug_prePos + (_debug_reflect * 20.0f);
        D2D1_POINT_2F end(endPos.x, endPos.y);
        renderTarget->DrawLine(pos, end, brushGreen, 2.0f);

        // 화살표 머리 만들기 (두 개의 짧은 선)
        D2D1_POINT_2F dir = { _debug_reflect.x, _debug_reflect.y };
        float arrowSize = 8.0f;

        // 수직 회전된 벡터
        D2D1_POINT_2F perp = { -dir.y, dir.x };

        D2D1_POINT_2F arrowLeft = {
            end.x - dir.x * arrowSize + perp.x * (arrowSize * 0.5f),
            end.y - dir.y * arrowSize + perp.y * (arrowSize * 0.5f)
        };

        D2D1_POINT_2F arrowRight = {
            end.x - dir.x * arrowSize - perp.x * (arrowSize * 0.5f),
            end.y - dir.y * arrowSize - perp.y * (arrowSize * 0.5f)
        };

        renderTarget->DrawLine(end, arrowLeft, brushGreen, 1.5f);
        renderTarget->DrawLine(end, arrowRight, brushGreen, 1.5f);
    }
    {
        D2D1_POINT_2F pos(_debug_prePos.x, _debug_prePos.y);
        Vector endPos = _debug_prePos + (_debug_normal * 20.0f);
        D2D1_POINT_2F end(endPos.x, endPos.y);
        renderTarget->DrawLine(pos, end, brushRed, 2.0f);

        // 화살표 머리 만들기 (두 개의 짧은 선)
        D2D1_POINT_2F dir = { _debug_normal.x, _debug_normal.y };
        float arrowSize = 8.0f;

        // 수직 회전된 벡터
        D2D1_POINT_2F perp = { -dir.y, dir.x };

        D2D1_POINT_2F arrowLeft = {
            end.x - dir.x * arrowSize + perp.x * (arrowSize * 0.5f),
            end.y - dir.y * arrowSize + perp.y * (arrowSize * 0.5f)
        };

        D2D1_POINT_2F arrowRight = {
            end.x - dir.x * arrowSize - perp.x * (arrowSize * 0.5f),
            end.y - dir.y * arrowSize - perp.y * (arrowSize * 0.5f)
        };

        renderTarget->DrawLine(end, arrowLeft, brushRed, 1.5f);
        renderTarget->DrawLine(end, arrowRight, brushRed, 1.5f);
    }
}

RenderLayer Ball::GetRenderLayer()
{
    return RenderLayer::RL_Ball;
}

bool Ball::OnBeginOverlapActor(Actor* other)
{
    if (Star* star = dynamic_cast<Star*>(other))
    {
        star->Destory();
        return true;
    }

    return false;
}
