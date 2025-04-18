#include "pch.h"
#include "Bullet.h"
#include "ResourceBase.h"
#include "Scene.h"
#include "Sprite.h"

Bullet::Bullet(Pos pos) : Super(pos)
{
}

Bullet::~Bullet()
{
}

void Bullet::Init()
{

}

void Bullet::Update(float deltaTime)
{
	_pos.y += _moveSpeed * deltaTime * _dir.yDir;

	if (_pos.y < 0)
	{
		// 화면 밖으로 나가면 삭제 예약
		Scene::GetInstance()->ReserveRemove(this);
	}

	_renderer.Update(deltaTime);
	_collider.Update();
}

void Bullet::Render(HDC hdc)
{
	_renderer.Render(hdc, _pos);
	_collider.Render(hdc);
}

void Bullet::SetResource(Sprite* texture)
{
	if (texture == nullptr)
		return;

	_renderer.SetSprite(texture, 0.2f);

	Size size = texture->GetSize();
	_pos.x -= (size.w * 0.5f);

    // 원의 중심과 반지름 설정
    _collider.Init(this, texture->GetSize(), _pos);
}
