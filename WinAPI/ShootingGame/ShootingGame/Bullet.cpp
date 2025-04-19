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
	Super::Init();
}

void Bullet::Update(float deltaTime)
{
	AddPosDelta(0, _moveSpeed * deltaTime * _dir.yDir);

	if (GetPos().y < 0 || GetPos().y > GWinSizeY)
	{
		// ȭ�� ������ ������ ���� ����
		Scene::GetInstance()->ReserveRemove(this);
	}

	_renderer.Update(deltaTime);
	_collider.Update();
}

void Bullet::Render(HDC hdc)
{
	_renderer.Render(hdc, GetPos());
	_collider.Render(hdc);
}

void Bullet::SetResource(Sprite* texture)
{
	if (texture == nullptr)
		return;

	_renderer.SetSprite(texture, 0.1f);

	Size size = texture->GetSize();
	AddPosDelta(-(size.w * 0.5f), 0);

    // ���� �߽ɰ� ������ ����
    _collider.Init(this, texture->GetSize(), GetPos());
}
