#include "pch.h"
#include "Airplane.h"
#include "Texture.h"

Airplane::Airplane(Pos pos) : Super(pos)
{
}

Airplane::~Airplane()
{
}


void Airplane::Init()
{
}

void Airplane::Update(float deltaTime)
{
	_collider.Update();
}

void Airplane::Render(HDC hdc)
{
	if (_texture == nullptr)
		return;

	_texture->Render(hdc, _pos);

	// �浹üũ�� ���� �� �׸���
	_collider.Render(hdc);
}

void Airplane::SetTexture(Texture* texture)
{
	_texture = texture;

	if (texture == nullptr)
		return;

	Size size = texture->GetSize();
	_pos.x -= (size.w / 2);
	_pos.y += (size.h / 2);

	// ���� �߽ɰ� ������ ����
	_collider.Init(this, _texture->GetSize(), _pos);
}

Size Airplane::GetSize()
{
	if (_texture == nullptr)
		return Size{ 0, 0 };
	return _texture->GetSize();
}

