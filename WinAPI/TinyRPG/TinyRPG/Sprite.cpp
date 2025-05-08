#include "pch.h"
#include "Sprite.h"
#include "Game.h"
#include "ResourceManager.h"

Sprite::Sprite(wstring key)
{
	_bitmap = ResourceManager::GetInstance()->GetDXBitmap(key);

	_info.width = _bitmap->GetFrameSize().Width;
	_info.height = _bitmap->GetFrameSize().Height;
}

Sprite::~Sprite()
{
	
}

void Sprite::Render(ID2D1RenderTarget* renderTarget, Vector pos)
{
	// 소스 비트맵에서 복사할 시작 좌표 계산
	float srcX = _info.indexX * (float)_bitmap->GetFrameSize().Width;
	float srcY = _info.indexY * (float)_bitmap->GetFrameSize().Height;

	// 원본 비트맵에서 그릴 영역 (소스 영역)
	D2D1_RECT_F srcRect = D2D1::RectF(
		srcX + 1.0f,
		srcY + 1.0f,
		srcX + _bitmap->GetFrameSize().Width - 1.0f,
		srcY + _bitmap->GetFrameSize().Height - 1.0f);

	Vector renderPos = _info.applyCamera ? Game::ConvertScreenPos(pos) : pos;

	// 화면에 렌더링할 위치와 크기 (대상 영역)
	D2D1_RECT_F destRect;
	if (_info.alignCenter)
	{
		destRect = D2D1::RectF(
			roundf(renderPos.x - _info.width * 0.5f),
			roundf(renderPos.y - _info.height * 0.5f),
			roundf(renderPos.x + _info.width * 0.5f),
			roundf(renderPos.y + _info.height * 0.5f));
	}
	else
	{
		destRect = D2D1::RectF(
			roundf(renderPos.x),
			roundf(renderPos.y),
			roundf(renderPos.x + _info.width),
			roundf(renderPos.y + _info.height));
	}

	// 기존 변환 상태 저장
	D2D1::Matrix3x2F originalTransform;
	renderTarget->GetTransform(&originalTransform);

	D2D1::Matrix3x2F finalTransform = originalTransform;
	D2D1::Matrix3x2F scaleTransform = D2D1::Matrix3x2F::Scale(
		D2D1::SizeF(_info.scale, _info.scale),
		D2D1::Point2F(renderPos.x, renderPos.y) // 반전 기준점
	);
	finalTransform = scaleTransform * finalTransform;

	// 좌우 반전 변환 행렬 설정
	if (_info.dirX < 0) // dirX가 음수일 경우 좌우 반전
	{
		D2D1::Matrix3x2F flipTransform = D2D1::Matrix3x2F::Scale(
			D2D1::SizeF(-1.0f, 1.0f), // X축 반전, Y축 그대로
			D2D1::Point2F(renderPos.x, renderPos.y) // 반전 기준점
		);
		finalTransform = flipTransform * finalTransform;
	}
	// 회전 행렬 설정
	if (_info.rotate != 0)
	{
		D2D1::Matrix3x2F rotate = D2D1::Matrix3x2F::Rotation(
			_info.rotate,
			D2D1::Point2F(renderPos.x, renderPos.y)
		);
		finalTransform = rotate * finalTransform;
	}

	// 최종 변환
	renderTarget->SetTransform(finalTransform);

	// 비트맵 렌더링
	renderTarget->DrawBitmap(_bitmap->GetBitmap(), destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, &srcRect);

	// 원래 변환 상태 복원
	renderTarget->SetTransform(originalTransform);

}

void Sprite::SetInfo(const SpriteRenderInfo& info)
{
	_info = info;
	if (info.width == 0)
		_info.width = _bitmap->GetFrameSize().Width;
	if (info.height == 0)
		_info.height = _bitmap->GetFrameSize().Height;
}

NumberSprite::NumberSprite(wstring key) : Super(key)
{
}

NumberSprite::~NumberSprite()
{
}

void NumberSprite::Render(ID2D1RenderTarget* renderTarget, Vector pos)
{
	// 123 
	// 3 -> 123 % 10 = 3
	// 2 -> (123 / 10) % 10 
	// 1 -> (123 / 100) % 10

	// 문자열 변환
	// string str = std::to_string(number);
	int32 tempNumber = _number;
	for (int32 i = 0; i < _numberPos.size(); ++i)
	{
		int32 number = tempNumber % 10;
		tempNumber = tempNumber / ((i + 1) * 10);

		_info.indexX = number;
		Super::Render(renderTarget, pos + _numberPos[i]);
	}
}

void NumberSprite::SetNumber(int8 number)
{
	// 숫자 10단위마다 위치값을 조절한다
	if (_number != number)
	{
		int32 count = number / 10;
		_numberPos.clear();
		for (int32 i = count; i >= 0; i--)
		{
			_numberPos.push_back(Vector(i * _info.width, 0));
		}
	}

	_number = number;
}
