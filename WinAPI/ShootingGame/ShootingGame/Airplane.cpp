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
	Super::Init();
}

void Airplane::Update(float deltaTime)
{
	_collider.Update();
}

void Airplane::Render(HDC hdc)
{
	if (_texture == nullptr)
		return;

	_texture->Render(hdc, GetPos());

	// 충돌체크를 위한 원 그리기
	_collider.Render(hdc);
}

void Airplane::SetTexture(Texture* texture)
{
	_texture = texture;

	if (texture == nullptr)
		return;

	Size size = texture->GetSize();
	AddPosDelta(-size.w / 2.0f, size.h / 2.0f);
	
	// 원의 중심과 반지름 설정
	_collider.Init(this, _texture->GetSize(), GetPos());
}

Size Airplane::GetSize()
{
	if (_texture == nullptr)
		return Size{ 0, 0 };
	return _texture->GetSize();
}

