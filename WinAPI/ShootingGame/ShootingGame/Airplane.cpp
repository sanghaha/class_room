#include "pch.h"
#include "Airplane.h"
#include "Texture.h"
#include "ImageRenderer.h"

Airplane::Airplane()
{
}

Airplane::~Airplane()
{
}

void Airplane::Init(Pos pos, wstring bitmapKey, bool checkCell)
{
	SetPos(pos, false);

	if (_collider == nullptr)
	{
		ImageRenderer* texture = CreateTextureComponent(bitmapKey);
		if (texture)
		{
			// 텍스쳐의 크기를 가져온다
			_size = texture->GetSize();

			// 원의 중심과 반지름 설정
			_collider = CreateColliderCircleComponent(_size, checkCell);

			AddPosDelta(-_size.w / 2.0f, _size.h / 2.0f, false);
		}
	}

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
