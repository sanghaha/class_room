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
}

void Projectile::Init()
{
    Super::Init();
}

void Projectile::Update(float deltaTime)
{
    // ���������� ���ư����Ѵ�.
    if (false == AddPosDelta(deltaTime))
    {
        // �̵� �Ϸ��ߴ��� Ȯ��
        if (_dest == GetPosCell())
        {
            // ���������
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

void Projectile::Render(ID2D1HwndRenderTarget* renderTarget)
{
    _renderer.Render(renderTarget, GetPos());
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

    Sprite* sprite = ResourceManager::GetInstance()->GetSprite(L"Arrow");
    _renderer.SetSprite(sprite, info);
}
