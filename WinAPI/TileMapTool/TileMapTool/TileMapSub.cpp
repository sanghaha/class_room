#include "pch.h"
#include "TileMapSub.h"
#include "ToolManager.h"
#include "InputManager.h"

void TileMapSub::Init(HWND hwnd, std::wstring path)
{
	::GetClientRect(hwnd, &_rect);

	_hdc = ::GetDC(hwnd);
	
	// ���� ���۸��� ���Ѱ�
	{
		_hdcBack = ::CreateCompatibleDC(_hdc); // hdc�� ȣȯ�Ǵ� DC�� ����
		_bmpBack = ::CreateCompatibleBitmap(_hdc, _rect.right, _rect.bottom); // hdc�� ȣȯ�Ǵ� ��Ʈ�� ����
		HBITMAP prev = (HBITMAP)::SelectObject(_hdcBack, _bmpBack); // DC�� BMP�� ����
		::DeleteObject(prev);
	}

	// Ÿ�ϸ� �ؽ�ó
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
		// Ŭ���� �׸��� ��ġ ���
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
	DrawTileMap(_hdcBack); // �� ���ۿ� �׸���

	{
		int x = _selectedIndex % TileMapWidth;
		int y = _selectedIndex / TileMapWidth;

		std::wstring str = std::format(L"x:{0}, y:{1}", x, y);
		::TextOut(_hdcBack, 5, 10, str.c_str(), static_cast<int>(str.size()));
	}

	// Double Buffering
	::BitBlt(_hdc, 0, 0, _rect.right, _rect.bottom, _hdcBack, 0, 0, SRCCOPY); // ��Ʈ �� : ��� ����
	::PatBlt(_hdcBack, 0, 0, _rect.right, _rect.bottom, WHITENESS);
}

void TileMapSub::DrawTileMap(HDC hdc)
{
	//if (GetInstance()->GetTileBitmap() == NULL)
	//{
	//	// Ÿ�ϸ��� ������ ������ �簢������ ǥ��
	//	for (int y = 0; y < TileMapHeight; y++) 
	//	{
	//		for (int x = 0; x < TileMapWidth; x++) 
	//		{
	//			RECT rect = { x * TileSize, y * TileSize,
	//					   (x + 1) * TileSize, (y + 1) * TileSize };

	//			HBRUSH hBrush = CreateSolidBrush(RGB(rand() % 200 + 55, rand() % 200 + 55, rand() % 200 + 55));
	//			FillRect(hdc, &rect, hBrush);
	//			DeleteObject(hBrush);

	//			// ���õ� Ÿ�� ����
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

		// ���õ� Ÿ�� ����
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

