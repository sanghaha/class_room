#include "pch.h"
#include "Ball.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "Sprite.h"
#include "Game.h"
#include "GameScene.h"
#include "Star.h"
#include "SoundManager.h"

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
    mass = 1.0f;               // 질량 (고정값)

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
    if (InputManager::GetInstance()->GetButtonPressed(KeyType::SpaceBar))
    {
        acceleration.y += moveForce / mass;
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
    //acceleration.y += gravity;
    acceleration += gravityVec;

    // 속도에 가속도 적용
    velocity += acceleration * deltaTime;

    float gravityFactor = 400;	// 양옆이동
    float sideFactor = 150;	// 양옆이동

    bool applyVectorBreak = true;

    // 낙하에 대해서도 고정된 값을 주고 싶다면 적용
    if (!applyVectorBreak)
    {
		if (velocity.y > gravityFactor)
			velocity.y = gravityFactor;
		if (velocity.y < -gravityFactor)
			velocity.y = -gravityFactor;

		if (velocity.x > sideFactor)
			velocity.x = sideFactor;
		if (velocity.x < -sideFactor)
			velocity.x = -sideFactor;

        // 좌우 마찰력을 줘서 좌우 방향으로는 점점 멈추게 한다
        velocity.x *= 0.95f;
        if (std::abs(velocity.x) < 1.0f) // 0에 수렴하니깐 임계값 크기만큼 줄어들면 0으로 변경
            velocity.x = 0.0f;
	}

    // 중력의 축 기준으로 속도를 제한한다.
    if(applyVectorBreak)
    {
        Vector gravityDir = gravityVec.GetNormalize();
        // 속도 벡터를 중력 방향기준으로 크기를 구한다.
        float dot = velocity.Dot(gravityDir);
        gravityDir = gravityDir * dot;

        Vector SideGravity = velocity - gravityDir;

        // y축 제한
        if (gravityFactor < dot)
        {
            gravityDir = gravityDir.GetNormalize() * gravityFactor;
        }

        if (sideFactor < SideGravity.Length())
        {
            SideGravity.Normalize();
            SideGravity *= sideFactor;
        }

        // 좌우 마찰력을 줘서 좌우 방향으로는 점점 멈추게 한다
        SideGravity *= 0.95f;
        if (SideGravity.Length() < 1.0f) // 0에 수렴하니깐 임계값 크기만큼 줄어들면 0으로 변경
            SideGravity = Vector(0, 0);

        velocity = gravityDir + SideGravity;
    }

    //if (velocity.x != 0.f)
    //{
    //    float sign = (velocity.x > 0) ? -1.f : 1.f;
    //    float frictionVel = sign * xFriction * mass * deltaTime;

    //    if (std::abs(velocity.x) <= std::abs(frictionVel))
    //    {
    //        // 마찰력보다 속도가 작다면 멈춘다
    //        velocity.x = 0;
    //    }
    //    else
    //    {
    //        velocity.x += frictionVel;
    //    }
    //}




    // 위치 갱신
    Vector newPos = GetPos() + velocity * deltaTime;

    OutputDebugStringW(std::format(L"vel:{0},{1}, acc:{2},{3}, prevPos:{4},{5} newPos:{6},{7}\n", velocity.x, velocity.y, acceleration.x, acceleration.y, GetPos().x, GetPos().y, newPos.x, newPos.y).c_str());

    // 바닥 충돌 체크
    Vector dir = newPos - GetPos();
    dir.Normalize();

    Vector normal; // 법선 벡터
    Vector hitPos;

    if (scene->CheckCollision(this, GetPos(), newPos, normal, hitPos))
    {
        Vector initVelocity = velocity.GetNormalize() * gravityFactor;
        //Vector initVelocity = velocity;

        // 반사 벡터 계산. velocity 를 반사벡터로 계산하면, 
        // 진행중인 속도에 비례해서 반사를 해주기 때문에, 계단 올라갈때 튕기는 속도가 다를수 있다.
        Vector reflect = initVelocity + (normal * 2.0f * -initVelocity.Dot(normal));
        _debug_reflect = reflect;
        //_debug_reflect.Normalize();

        // 좌우 벽에 부딪힌 상태면 그냥 x축 보정만 해준다.
        //if (normal.x != 0)
        //{
        //    velocity.x = reflect.x;
        //}
        
        //if(normal.y < 0)
        //{
        //    velocity.y = -upFactor;
        //}

        const float epsilon = 10.0f;
        newPos = hitPos + (normal * epsilon);

        velocity = reflect;
        OutputDebugStringW(std::format(L"[Collision] vel:{0},{1}, hitPos:{2}{3}, normal:{4},{5}\n", 
            velocity.x, velocity.y, hitPos.x, hitPos.y, normal.x, normal.y).c_str());
        
        _debug_prePos = GetPos(); // -(dir * GetRadius());
        _debug_normal = normal;
        _debug_newPos = newPos;

        //ResourceManager::GetInstance()->PlayWAVSound("Bauns.wav");
		SoundManager::GetInstance()->Play(L"Bauns");
    }

    SetPos(newPos);

    // 좌표가 Window Y 값을 넘어가면 죽음 처리
    if (newPos.y >= GWinSizeY)
    {
        GameScene* gameScene = dynamic_cast<GameScene*>(Game::GetInstance()->GetScene());
        if (gameScene)
        {
            gameScene->Dead(GetPos());
        }
    }
}

void Ball::Render(ID2D1RenderTarget* renderTarget)
{
    Super::Render(renderTarget);

    static int32 MaxVelocity = 0;
    if (velocity.Length() > MaxVelocity)
        MaxVelocity = velocity.Length();



    auto brushRed = ResourceManager::GetInstance()->GetBrush(BrushColor::Red);
    auto brushBlue = ResourceManager::GetInstance()->GetBrush(BrushColor::Blue);
    auto brushGreen = ResourceManager::GetInstance()->GetBrush(BrushColor::Green);

    auto font = ResourceManager::GetInstance()->GetFont(FontSize::FONT_12);
    wstring str = std::format(L"vel:{0},ref:{1},maxVel:{2}", (int32)velocity.Length(), (int32)_debug_reflect.Length(), MaxVelocity);
    renderTarget->DrawTextW(
        str.c_str(),
        (uint32)str.size(),
        font,
        D2D1::RectF(350, 500, 350 + 200, 500 + 100),
        brushRed
    );

    {
        D2D1_ELLIPSE dot = D2D1::Ellipse(D2D1_POINT_2F(_debug_prePos.x, _debug_prePos.y), 3, 3);
        renderTarget->FillEllipse(dot, brushRed);
    }
    {
        D2D1_ELLIPSE dot = D2D1::Ellipse(D2D1_POINT_2F(_debug_newPos.x, _debug_newPos.y), 3, 3);
        renderTarget->FillEllipse(dot, brushBlue);
    }
    {
        Vector reflectNormal = _debug_reflect;
        reflectNormal.Normalize();
        D2D1_POINT_2F pos(_debug_prePos.x, _debug_prePos.y);
        Vector endPos = _debug_prePos + (reflectNormal * 20.0f);
        D2D1_POINT_2F end(endPos.x, endPos.y);
        renderTarget->DrawLine(pos, end, brushGreen, 2.0f);

        // 화살표 머리 만들기 (두 개의 짧은 선)
        D2D1_POINT_2F dir = { reflectNormal.x, reflectNormal.y };
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

float Ball::GetRadius()
{
    return BALL_SIZE / 2.f;
}

bool Ball::OnBeginOverlapActor(Actor* other)
{
    if (Star* star = dynamic_cast<Star*>(other))
    {
        GameScene* gameScene = dynamic_cast<GameScene*>(Game::GetInstance()->GetScene());
        gameScene->AddStarCount(GetPos());

        star->Destroy();
        return true;
    }

    return false;
}
