#include "pch.h"
#include "Ball.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "Game.h"
#include "GameScene.h"
#include "Star.h"
#include "SoundManager.h"
#include "SpriteRenderer.h"

Ball::Ball(Vector pos) : Super(pos)
{
}

Ball::~Ball()
{
}

void Ball::Init()
{
    Super::Init();

    CreateSpriteComponent("Ball_0", BALL_SIZE, BALL_SIZE);
	CreateRectCollider(BALL_SIZE, BALL_SIZE);
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
    if (InputManager::GetInstance()->GetButtonDown(KeyType::SpaceBar))
    {
        if (onGround)
        {
            acceleration.y -= moveForce * 100 / mass;
            onGround = false; // SpaceBar를 누르면 공이 점프하도록 설정
            OutputDebugStringW(std::format(L"Right Key Pressed\n").c_str());;
        }
    }

    applyPhysics(deltaTime);

    // 바닥 판단
    //checkFloor();
}

const float epsilon = 5.0f;

void Ball::checkFloor()
{
    GameScene* scene = dynamic_cast<GameScene*>(Game::GetInstance()->GetScene());
    if (nullptr == scene)
        return;

    // 중력 방향으로 길게 선분 검사를 수행
	Vector newPos = GetPos() + (gravityVec * 1000.0f);
    Vector normal;
    Vector hitPos;

    if (scene->CheckCollision(this, GetPos(), newPos, normal, hitPos))
    {
        // 충돌 지점과 거리 계산
        Vector move = hitPos - GetPos();
        float dist = move.Length();

		if (dist < 1)
		{
			onGround = true;
			
            //SetPos(hitPos);
			OutputDebugStringW(std::format(L"[CheckFloor] onGround: true, pos:{0},{1}\n", hitPos.x, hitPos.y).c_str());
		}
		else
		{
			onGround = false;
			//OutputDebugStringW(std::format(L"[CheckFloor] onGround: false\n").c_str());
		}
    }
}

void Ball::applyPhysics(float deltaTime)
{
    GameScene* scene = dynamic_cast<GameScene*>(Game::GetInstance()->GetScene());
    if (nullptr == scene)
        return;

    // 물리 효과 적용
    // 속도에 중력 적용
    acceleration += gravityVec;

    // 만약에 땅위에 서있다면 중력을 더하지 않고, 중력 방향으로 속도를 제거한다.
	//if (onGround)
	//{
	//	// 중력 방향으로 속도를 제거한다.
 //       Vector gravity = gravityVec;
 //       gravity.Normalize();

 //       float dot = velocity.Dot(gravity);
 //       velocity -= gravity * dot;
	//}
 //   else
 //   {
 //       //acceleration.y += gravity;
 //       acceleration += gravityVec;
 //   }

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

    // 충돌 체크
    Vector dir = newPos - GetPos();
    dir.Normalize();

    Vector normal; // 법선 벡터
    Vector hitPos;

    if (scene->CheckCollision(this, GetPos(), newPos, normal, hitPos))
    {
        Vector initVelocity = velocity.GetNormalize() * gravityFactor;

        // 반사 벡터 계산. velocity 를 반사벡터로 계산하면, 
        // 진행중인 속도에 비례해서 반사를 해주기 때문에, 계단 올라갈때 튕기는 속도가 다를수 있다.
        Vector reflect = initVelocity + (normal * 2.0f * -initVelocity.Dot(normal));
        _debug_reflect = reflect;

        //Vector gravityDir = gravityVec.GetNormalize();

        //Vector up(0, -1);
        //Vector vn45 = up.Rotate(-45);
        //Vector v45 = up.Rotate(45);

        //float dotn45 = gravityDir.Dot(vn45);
        //float dot45 = gravityDir.Dot(v45);

        //float dot = normal.Dot(gravityDir);
        //if (normal.y < 0)
        //{
        //    onGround = true;
        //}
        //else
        //{
        //    onGround = false;
        //}
        //_debug_reflect.Normalize();
        newPos = hitPos;
        
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

void Ball::Render(HDC renderTarget)
{
    Super::Render(renderTarget);

    //Vector up(0, -1);
    //Vector vn45 = up.Rotate(-45);
    //Vector v45 = up.Rotate(45);

    //Vector end1 = GetPos() + vn45 * 50;
    //Vector end2 = GetPos() + v45 * 50;

    //{
    //    HPEN myPen = CreatePen(PS_SOLID, 3, RGB(0, 255, 255));
    //    HPEN oldPen = (HPEN)SelectObject(renderTarget, myPen);

    //    MoveToEx(renderTarget, GetPos().x, GetPos().y, nullptr);
    //    LineTo(renderTarget, end1.x, end1.y);

    //    // 이전 펜 복원 및 새 펜 삭제
    //    SelectObject(renderTarget, oldPen);
    //    DeleteObject(myPen);
    //}
    //{
    //    HPEN myPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 255));
    //    HPEN oldPen = (HPEN)SelectObject(renderTarget, myPen);

    //    MoveToEx(renderTarget, GetPos().x, GetPos().y, nullptr);
    //    LineTo(renderTarget, end2.x, end2.y);

    //    // 이전 펜 복원 및 새 펜 삭제
    //    SelectObject(renderTarget, oldPen);
    //    DeleteObject(myPen);
    //}
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
