#include "pch.h"
#include "TileMapSub.h"
#include "ToolManager.h"
#include "InputManager.h"

void TileMapSub::Init(HWND hwnd, std::wstring path)
{
	::GetClientRect(hwnd, &_rect);

	_hdc = ::GetDC(hwnd);
	
	// 더블 버퍼링을 위한것
	{
		_hdcBack = ::CreateCompatibleDC(_hdc); // hdc와 호환되는 DC를 생성
		_bmpBack = ::CreateCompatibleBitmap(_hdc, _rect.right, _rect.bottom); // hdc와 호환되는 비트맵 생성
		HBITMAP prev = (HBITMAP)::SelectObject(_hdcBack, _bmpBack); // DC와 BMP를 연결
		::DeleteObject(prev);
	}

	// 타일맵 텍스처
	{
		_hdcBitmap = ::CreateCompatibleDC(_hdc);
		_bitmap = (HBITMAP)::LoadImageW(
			nullptr,
			path.c_str(),
			IMAGE_BITMAP,
			0,
			0,
			LR_LOADFROMFILE | LR_CREATEDIBSECTION
		);

		HBITMAP prev = (HBITMAP)::SelectObject(_hdcBitmap, _bitmap);
		::DeleteObject(prev);

		BITMAP bit = {};
		::GetObject(_bitmap, sizeof(BITMAP), &bit);

		_sizeX = bit.bmWidth;
		_sizeY = bit.bmHeight;
		_transparent = RGB(255, 0, 255);
	}
}

void TileMapSub::Update()
{
	if (InputManager::GetInstance()->GetButtonPressed(KeyType::LeftMouse))
	{
		POINT mousePos = InputManager::GetInstance()->GetSubMousePos();
		// 클릭한 그리드 위치 계산
		int x = mousePos.x / TileSize;
		int y = mousePos.y / TileSize;

		if (x >= 0 && x < TileMapWidth && y >= 0 && y < TileMapHeight) 
		{
			_selectedIndex = y * TileMapWidth + x;
		}
	}
}

void TileMapSub::Render()
{
	DrawTileMap(_hdcBack); // 백 버퍼에 그리기

	{
		int x = _selectedIndex % TileMapWidth;
		int y = _selectedIndex / TileMapWidth;

		std::wstring str = std::format(L"x:{0}, y:{1}", x, y);
		::TextOut(_hdcBack, 5, 10, str.c_str(), static_cast<int>(str.size()));
	}

	// Double Buffering
	::BitBlt(_hdc, 0, 0, _rect.right, _rect.bottom, _hdcBack, 0, 0, SRCCOPY); // 비트 블릿 : 고속 복사
	::PatBlt(_hdcBack, 0, 0, _rect.right, _rect.bottom, WHITENESS);
}

void TileMapSub::DrawTileMap(HDC hdc)
{
	//if (GetInstance()->GetTileBitmap() == NULL)
	//{
	//	// 타일맵이 없으면 간단한 사각형으로 표시
	//	for (int y = 0; y < TileMapHeight; y++) 
	//	{
	//		for (int x = 0; x < TileMapWidth; x++) 
	//		{
	//			RECT rect = { x * TileSize, y * TileSize,
	//					   (x + 1) * TileSize, (y + 1) * TileSize };

	//			HBRUSH hBrush = CreateSolidBrush(RGB(rand() % 200 + 55, rand() % 200 + 55, rand() % 200 + 55));
	//			FillRect(hdc, &rect, hBrush);
	//			DeleteObject(hBrush);

	//			// 선택된 타일 강조
	//			if (y * TileMapWidth + x == _selectedIndex)
	//			{
	//				HPEN hPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
	//				HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
	//				SelectObject(hdc, GetStockObject(NULL_BRUSH));
	//				Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
	//				SelectObject(hdc, hOldPen);
	//				DeleteObject(hPen);
	//			}
	//		}
	//	}
	//}
	//else 
	{
		::TransparentBlt(hdc,
			0,
			0,
			TileMapWidth * TileSize,
			TileMapHeight * TileSize,
			_hdcBitmap,
			0,
			0,
			_sizeX,
			_sizeY,
			_transparent);

		// 선택된 타일 강조
		if (_selectedIndex >= 0) 
		{
			int x = _selectedIndex % TileMapWidth;
			int y = _selectedIndex / TileMapWidth;

			HPEN hPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
			HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
			SelectObject(hdc, GetStockObject(NULL_BRUSH));
			Rectangle(hdc, x * TileSize, y * TileSize, (x + 1) * TileSize, (y + 1) * TileSize);
			SelectObject(hdc, hOldPen);
			DeleteObject(hPen);
		}
	}
}

