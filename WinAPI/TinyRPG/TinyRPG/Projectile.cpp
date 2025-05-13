#include "pch.h"
#include "Projectile.h"
#include "ResourceManager.h"
#include "Game.h"
#include "GameScene.h"

Projectile::Projectile(Vector pos) : Super(pos)
{
    SetMoveSpeed(600);

    // Sprite 생성
    _sprite = CreateSpriteComponent("Arrow");
}

Projectile::~Projectile()
{
}

void Projectile::Init()
{
    Super::Init();
}

void Projectile::Update(float deltaTime)
{
    Super::Update(deltaTime);

    // 목적지까지 날아가야한다.
    if (false == AddPosDelta(deltaTime))
    {
        // 이동 완료했는지 확인
        if (_dest == GetPosCell())
        {
            // 사라져야함
            Game::GetGameScene()->ReserveRemove(this);
        }
        else
        {
            Cell currCell = GetPosCell();
            Cell moveCell = currCell.NextCell(_dir);
            SetPosCell(moveCell);
        }
    }
}

void Projectile::Render(ID2D1RenderTarget* renderTarget)
{
    Super::Render(renderTarget);
}

RenderLayer Projectile::GetRenderLayer()
{
	return RenderLayer::RL_Projectile;
}

void Projectile::SetProjectileInfo(DirType dir, Cell dest, int32 attack)
{
    _dest = dest;
    _dir = dir;
    _attack = attack;

    Cell currCell = GetPosCell();
    Cell moveCell = currCell.NextCell(dir);
    SetPosCell(moveCell);

    if (_sprite)
    {
        _sprite->SetFlip((dir == DirType::DIR_LEFT) ? true : false);

		if (dir == DirType::DIR_DOWN)
			_sprite->SetRotate(90);
		else if (dir == DirType::DIR_UP)
			_sprite->SetRotate(-90);
    }
}
