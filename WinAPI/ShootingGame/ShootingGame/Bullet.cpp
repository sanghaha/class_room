#include "pch.h"
#include "Bullet.h"
#include "ResourceBase.h"
#include "Scene.h"
#include "Sprite.h"
#include "Game.h"
#include "Texture.h"

Bullet::Bullet(Pos pos, wstring bitmapKey, int32 indexX, BulletType type) : Super(pos, bitmapKey, 0), _type(type)
{
	if (_sprite)
	{
		_sprite->SetIndex(indexX, 0);

		// 텍스쳐의 크기를 가져온다
		Size size = _sprite->GetSize();

		// 원의 중심과 반지름 설정
		_collider = CreateColliderCircleComponent(size, (type == BulletType::BT_Player) ? true : false);
	}
}

Bullet::~Bullet()
{
}

void Bullet::Reset(Pos pos, int32 indexX)
{
	SetPos(pos);

	if (_sprite)
	{
		_sprite->SetIndex(indexX, 0);
	}
}

void Bullet::Update(float deltaTime)
{
	Super::Update(deltaTime);

	AddPosDelta(0, _moveSpeed * deltaTime * _dir.yDir);

	if (GetPos().y < 0 || GetPos().y > GWinSizeY)
	{
		// 화면 밖으로 나가면 삭제 예약
		Destroy();
	}
}

void Bullet::Render(HDC hdc)
{
	Super::Render(hdc);
}