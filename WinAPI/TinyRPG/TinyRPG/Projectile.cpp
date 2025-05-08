#include "pch.h"
#include "Projectile.h"
#include "ResourceManager.h"
#include "Game.h"
#include "GameScene.h"

Projectile::Projectile(Vector pos) : Super(pos)
{
    SetMoveSpeed(600);
}

Projectile::~Projectile()
{
    SAFE_DELETE(_renderer);
}

void Projectile::Init()
{
    Super::Init();
}

void Projectile::Update(float deltaTime)
{
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
    _renderer->Render(renderTarget, GetPos());
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

    SpriteRenderInfo info;
    info.indexX = 0;
    info.indexY = 0;
    info.dirX = (dir == DirType::DIR_LEFT) ? -1 : 1;

    if (dir == DirType::DIR_DOWN)
        info.rotate = 90;
    else if (dir == DirType::DIR_UP)
        info.rotate = -90;

    _renderer = new Sprite(L"Arrow");
    _renderer->SetInfo(info);
}
