#include "pch.h"
#include "Texture.h"
#include "Game.h"

BitmapTexture::BitmapTexture()
{
}

BitmapTexture::~BitmapTexture()
{
}

void BitmapTexture::Render(ID2D1HwndRenderTarget* renderTarget, Vector pos, int32 srcX, int32 srcY)
{
	//Vector renderPos = Game::ConvertRenderPos(pos);
	//if (_transparent == -1)
	//{
	//	::BitBlt(hdc,	// 백버퍼에
	//		(int32)renderPos.x,
	//		(int32)renderPos.y,
	//		_sizeX,
	//		_sizeY,
	//		_hdc,	// 텍스쳐 그리기
	//		srcX,
	//		srcY,
	//		SRCCOPY);
	//}
	//else
	//{
	//	::TransparentBlt(hdc,
	//		(int32)renderPos.x,
	//		(int32)renderPos.y,
	//		_sizeX,
	//		_sizeY,
	//		_hdc,
	//		srcX,
	//		srcY,
	//		_sizeX,
	//		_sizeY,
	//		_transparent);
	//}
}

void BitmapTexture::Load(HWND hwnd, wstring path, int32 transparent)
{
	/*_bitmap = (HBITMAP)::LoadImageW(
		nullptr,
		path.c_str(),
		IMAGE_BITMAP,
		0,
		0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION
	);

	if (_bitmap == 0)
	{
		::MessageBox(hwnd, path.c_str(), L"Invalid BitmapTexture Load", MB_OK);
		return;
	}

	HBITMAP prev = (HBITMAP)::SelectObject(_hdc, _bitmap);
	::DeleteObject(prev);

	BITMAP bit = {};
	::GetObject(_bitmap, sizeof(BITMAP), &bit);

	_sizeX = bit.bmWidth;
	_sizeY = bit.bmHeight;
	_transparent = transparent;*/
}

/// <summary>
/// UI 용 텍스쳐
/// </summary>
Sliced3Texture::Sliced3Texture()
{
}

Sliced3Texture::~Sliced3Texture()
{
}

void Sliced3Texture::Load(HWND hwnd, wstring path, int32 left, int32 right)
{
	// PNG 파일 로드
	//_image = new Image(path.c_str());
	//if (_image->GetLastStatus() != Ok)
	//{
	//	MessageBox(nullptr, L"Failed to load image", L"Error", MB_OK);

	//	delete _image;
	//	_image = nullptr;
	//	return;
	//}

	//// 이미지 크기 가져오기
	//_sizeX = _image->GetWidth();
	//_sizeY = _image->GetHeight();
	//_left = left;
	//_right = right;
}

void Sliced3Texture::Render(ID2D1HwndRenderTarget* renderTarget, Vector pos, int32 sizeX, int32 sizeY, float clipRatio)
{
	//if (!_image || sizeX <= 0 || _left + _right > sizeX || clipRatio < 0.0f || clipRatio > 1.0f)
	//	return;

	//// 클리핑 영역 계산 (비율에 따라 시작 픽셀부터 클리핑)
	//int32 clipWidth = static_cast<int32>(sizeX * clipRatio);
	//Rect clipRect((int32)pos.x, (int32)pos.y, clipWidth, sizeY);
	//graphics->SetClip(clipRect, CombineModeReplace);

	//// 1. Left 영역 렌더링
	//Rect srcLeft(0, 0, _left, _sizeY); // 소스 영역 (좌측)
	//Rect destLeft((int32)pos.x, (int32)pos.y, _left, sizeY); // 대상 영역
	//graphics->DrawImage(_image, destLeft, srcLeft.X, srcLeft.Y, srcLeft.Width, srcLeft.Height, UnitPixel);

	//// 2. Center 영역 렌더링
	//int32 centerWidth = sizeX - _left - _right; // 가운데 영역의 너비
	//if (centerWidth > 0)
	//{
	//	Rect srcCenter(_left, 0, _sizeX - _left - _right, _sizeY); // 소스 영역 (가운데)
	//	Rect destCenter((int32)pos.x + _left, (int32)pos.y, centerWidth, sizeY); // 대상 영역
	//	graphics->DrawImage(_image, destCenter, srcCenter.X, srcCenter.Y, srcCenter.Width, srcCenter.Height, UnitPixel);
	//}

	//// 3. Right 영역 렌더링
	//Rect srcRight(_sizeX - _right, 0, _right, _sizeY); // 소스 영역 (우측)
	//Rect destRight((int32)pos.x + sizeX - _right, (int32)pos.y, _right, sizeY); // 대상 영역
	//graphics->DrawImage(_image, destRight, srcRight.X, srcRight.Y, srcRight.Width, srcRight.Height, UnitPixel);

	//// 클리핑 해제
	//graphics->ResetClip();
}
