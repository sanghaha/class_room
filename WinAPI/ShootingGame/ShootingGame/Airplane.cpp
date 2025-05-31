#include "pch.h"
#include "Airplane.h"
#include "Texture.h"

Airplane::Airplane(Pos pos, wstring bitmapKey, bool checkCell) : Super(pos)
{
	Texture* texture = CreateTextureComponent(bitmapKey);
	if (texture)
	{
		// 텍스쳐의 크기를 가져온다
		_size = texture->GetSize();

		// 원의 중심과 반지름 설정
		_collider = CreateColliderCircleComponent(_size, checkCell);

		AddPosDelta(-_size.w / 2.0f, _size.h / 2.0f);
	}
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
	Super::Update(deltaTime);
}

void Airplane::Render(HDC hdc)
{
	Super::Render(hdc);
}

Pos Airplane::GetCenterPos()
{
	Pos center;
	center.x = GetPos().x + (_size.w * 0.5f);
	center.y = GetPos().y + (_size.h * 0.5f);
	return center;
}
