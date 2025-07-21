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
    float moveForce = 500.0f;        // ���� ũ��
    mass = 1.0f;               // ���� (������)

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

    // ���� ȿ�� ����
    // �ӵ��� �߷� ����
    //acceleration.y += gravity;
    acceleration += gravityVec;

    // �ӵ��� ���ӵ� ����
    velocity += acceleration * deltaTime;

    float gravityFactor = 400;	// �翷�̵�
    float sideFactor = 150;	// �翷�̵�

    bool applyVectorBreak = true;

    // ���Ͽ� ���ؼ��� ������ ���� �ְ� �ʹٸ� ����
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

        // �¿� �������� �༭ �¿� �������δ� ���� ���߰� �Ѵ�
        velocity.x *= 0.95f;
        if (std::abs(velocity.x) < 1.0f) // 0�� �����ϴϱ� �Ӱ谪 ũ�⸸ŭ �پ��� 0���� ����
            velocity.x = 0.0f;
	}

    // �߷��� �� �������� �ӵ��� �����Ѵ�.
    if(applyVectorBreak)
    {
        Vector gravityDir = gravityVec.GetNormalize();
        // �ӵ� ���͸� �߷� ����������� ũ�⸦ ���Ѵ�.
        float dot = velocity.Dot(gravityDir);
        gravityDir = gravityDir * dot;

        Vector SideGravity = velocity - gravityDir;

        // y�� ����
        if (gravityFactor < dot)
        {
            gravityDir = gravityDir.GetNormalize() * gravityFactor;
        }

        if (sideFactor < SideGravity.Length())
        {
            SideGravity.Normalize();
            SideGravity *= sideFactor;
        }

        // �¿� �������� �༭ �¿� �������δ� ���� ���߰� �Ѵ�
        SideGravity *= 0.95f;
        if (SideGravity.Length() < 1.0f) // 0�� �����ϴϱ� �Ӱ谪 ũ�⸸ŭ �پ��� 0���� ����
            SideGravity = Vector(0, 0);

        velocity = gravityDir + SideGravity;
    }

    //if (velocity.x != 0.f)
    //{
    //    float sign = (velocity.x > 0) ? -1.f : 1.f;
    //    float frictionVel = sign * xFriction * mass * deltaTime;

    //    if (std::abs(velocity.x) <= std::abs(frictionVel))
    //    {
    //        // �����º��� �ӵ��� �۴ٸ� �����
    //        velocity.x = 0;
    //    }
    //    else
    //    {
    //        velocity.x += frictionVel;
    //    }
    //}




    // ��ġ ����
    Vector newPos = GetPos() + velocity * deltaTime;

    OutputDebugStringW(std::format(L"vel:{0},{1}, acc:{2},{3}, prevPos:{4},{5} newPos:{6},{7}\n", velocity.x, velocity.y, acceleration.x, acceleration.y, GetPos().x, GetPos().y, newPos.x, newPos.y).c_str());

    // �ٴ� �浹 üũ
    Vector dir = newPos - GetPos();
    dir.Normalize();

    Vector normal; // ���� ����
    Vector hitPos;

    if (scene->CheckCollision(this, GetPos(), newPos, normal, hitPos))
    {
        Vector initVelocity = velocity.GetNormalize() * gravityFactor;
        //Vector initVelocity = velocity;

        // �ݻ� ���� ���. velocity �� �ݻ纤�ͷ� ����ϸ�, 
        // �������� �ӵ��� ����ؼ� �ݻ縦 ���ֱ� ������, ��� �ö󰥶� ƨ��� �ӵ��� �ٸ��� �ִ�.
        Vector reflect = initVelocity + (normal * 2.0f * -initVelocity.Dot(normal));
        _debug_reflect = reflect;
        //_debug_reflect.Normalize();

        // �¿� ���� �ε��� ���¸� �׳� x�� ������ ���ش�.
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

    // ��ǥ�� Window Y ���� �Ѿ�� ���� ó��
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

        // ȭ��ǥ �Ӹ� ����� (�� ���� ª�� ��)
        D2D1_POINT_2F dir = { reflectNormal.x, reflectNormal.y };
        float arrowSize = 8.0f;

        // ���� ȸ���� ����
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

        // ȭ��ǥ �Ӹ� ����� (�� ���� ª�� ��)
        D2D1_POINT_2F dir = { _debug_normal.x, _debug_normal.y };
        float arrowSize = 8.0f;

        // ���� ȸ���� ����
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
