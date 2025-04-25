#include "pch.h"
#include "TileMapMain.h"
#include "TileMapTool.h"
#include "InputManager.h"
#include "ToolManager.h"
#include <fstream>

void TileMapMain::Init(HWND hwnd, std::wstring path)
{
	::GetClientRect(hwnd, &_rect);

	_hwnd = hwnd;
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


	// �׸��� �ʱ�ȭ
	for (int i = 0; i < _layerCount; i++)
		_layer[i].mainGrid.resize(GridWidth * GridHeight, -1);

	_validLayerTiles[0].emplace_back(Water);
	_validLayerTiles[1].emplace_back(Rock);
	_validLayerTiles[2].emplace_back(Ground);
	_validLayerTiles[2].emplace_back(Bridge1);
	_validLayerTiles[2].emplace_back(Bridge2);
}

void TileMapMain::Update()
{
	if (InputManager::GetInstance()->GetButtonDown(KeyType::LeftMouse))
	{
		_isDragging = true;
	}

	if (_isDragging && InputManager::GetInstance()->GetButtonPressed(KeyType::LeftMouse))
	{
		POINT mousePos = InputManager::GetInstance()->GetMainMousePos();
		// Ŭ���� �׸��� ��ġ ���
		int x = mousePos.x / TileSize;
		int y = mousePos.y / TileSize;

		// �׸��� ���� Ȯ��
		if (x >= 0 && x < GridWidth && y >= 0 && y < GridHeight && ToolManager::GetInstance()->GetSelectedTileIndex() >= 0)
		{
			// ���õ� Ÿ���� �׸��忡 ��ġ
			if (IsValidTile(ToolManager::GetInstance()->GetSelectedTileIndex()))
			{
				_layer[_selectedLayer].mainGrid[y * GridWidth + x] = ToolManager::GetInstance()->GetSelectedTileIndex();
			}
		}
	}

	if (InputManager::GetInstance()->GetButtonDown(KeyType::RightMouse))
	{
		POINT mousePos = InputManager::GetInstance()->GetMainMousePos();
		// Ŭ���� �׸��� ��ġ ���
		int x = mousePos.x / TileSize;
		int y = mousePos.y / TileSize;

		// �׸��� ���� Ȯ��
		if (x >= 0 && x < GridWidth && y >= 0 && y < GridHeight && ToolManager::GetInstance()->GetSelectedTileIndex() >= 0)
		{
			// ���õ� Ÿ���� �׸��忡 ��ġ
			_layer[_selectedLayer].mainGrid[y * GridWidth + x] = -1;
		}
	}

	if (InputManager::GetInstance()->GetButtonUp(KeyType::LeftMouse))
	{
		_isDragging = false;
	}

	if (InputManager::GetInstance()->GetButtonUp(KeyType::S))
	{
		SaveTileMap();
	}
	if (InputManager::GetInstance()->GetButtonUp(KeyType::L))
	{
		LoadTileMap();
	}
	if (InputManager::GetInstance()->GetButtonUp(KeyType::Q))
	{
		_selectedLayer = min(_layerCount - 1, _selectedLayer + 1);
	}
	if (InputManager::GetInstance()->GetButtonUp(KeyType::E))
	{
		_selectedLayer = max(0, _selectedLayer - 1);
	}
	if (InputManager::GetInstance()->GetButtonUp(KeyType::F1))
	{
		_isDrawCurrLayer = !_isDrawCurrLayer;
	}
}

void TileMapMain::Render()
{
	DrawMainGrid(_hdcBack); // �� ���ۿ� �׸���

	{
		std::wstring str = std::format(L"Layer:{0}, State:{1}", _selectedLayer, _isDrawCurrLayer);
		//::TextOut(_hdcBack, 5, 10, str.c_str(), static_cast<int>(str.size()));
	}


	// Double Buffering
	::BitBlt(_hdc, 0, 0, _rect.right, _rect.bottom, _hdcBack, 0, 0, SRCCOPY); // ��Ʈ �� : ��� ����
	::PatBlt(_hdcBack, 0, 0, _rect.right, _rect.bottom, WHITENESS);
}

void TileMapMain::DrawMainGrid(HDC hdc)
{
	HPEN hPen = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
	HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

	// ���� �׸���
	for (int x = 0; x <= GridWidth; x++) {
		MoveToEx(hdc, x * TileSize, 0, NULL);
		LineTo(hdc, x * TileSize, GridHeight * TileSize);
	}

	for (int y = 0; y <= GridHeight; y++) {
		MoveToEx(hdc, 0, y * TileSize, NULL);
		LineTo(hdc, GridWidth * TileSize, y * TileSize);
	}

	SelectObject(hdc, hOldPen);
	DeleteObject(hPen);

	// Ÿ�� �׸���
	for (int i = 0; i < _layerCount; i++) 
	{
		if (_isDrawCurrLayer && i != _selectedLayer)
			continue;

		// ���̾ Ÿ�� �׸���
		for (int y = 0; y < GridHeight; y++) {
			for (int x = 0; x < GridWidth; x++) {
				int tileIndex = _layer[i].mainGrid[y * GridWidth + x];
				if (tileIndex >= 0) {
					DrawTileOnGrid(hdc, i, x, y);
				}
			}
		}
	}
}

void TileMapMain::DrawTileOnGrid(HDC hdc, int layer, int gridX, int gridY)
{
	int tileIndex = _layer[layer].mainGrid[gridY * GridWidth + gridX];

	// ������ Ÿ���� �׸��忡 �׸���
	int tileX = tileIndex % TileMapWidth;
	int tileY = tileIndex / TileMapWidth;

	// ���(RGB(255, 255, 255))�� ��������� ó���Ͽ� ��Ʈ�� �׸���
	TransparentBlt(
		hdc,
		gridX * TileSize, gridY * TileSize, TileSize, TileSize, // ��� ��ġ �� ũ��
		_hdcBitmap,
		tileX * TileSize, tileY * TileSize, TileSize, TileSize, // ���� ��ġ �� ũ��
		_transparent
	);
}


void TileMapMain::SaveTileMap() {
	OPENFILENAME ofn;
	wchar_t szFileName[MAX_PATH] = L"";

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = _hwnd;
	ofn.lpstrFilter = L"Ÿ�ϸ� ���� (*.tilemap)\0*.tilemap\0��� ���� (*.*)\0*.*\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	ofn.lpstrDefExt = L"tilemap";

	if (GetSaveFileName(&ofn)) {
		// ���� �̸��� ���õǾ����� ����
		std::wstring wFileName = szFileName;
		std::wstring fileName(wFileName.begin(), wFileName.end());

		std::wofstream file(fileName);
		if (file.is_open()) {
			// �׸��� ũ�� ����
			file << GridWidth << "," << GridHeight << "," << TileMapWidth << "," << TileMapHeight << std::endl;

			// Ÿ�� ������ ����
			for (int i = 0; i < _layerCount; i++)
			{
				int size = _layer[i].GetValidCount();
				file << i << ":" << size << std::endl;
				for (int y = 0; y < GridHeight; y++) 
				{
					for (int x = 0; x < GridWidth; x++) 
					{
						int tileIndex = _layer[i].mainGrid[y * GridWidth + x];
						if (tileIndex >= 0)
						{
							file << x << "," << y << "," << tileIndex << std::endl;
						}
					}
				}
			}

			file.close();
			MessageBox(_hwnd, L"Ÿ�ϸ��� ����Ǿ����ϴ�.", L"���� �Ϸ�", MB_OK | MB_ICONINFORMATION);
		}
		else {
			MessageBox(_hwnd, L"������ ������ �� �����ϴ�.", L"����", MB_OK | MB_ICONERROR);
		}
	}
}

void TileMapMain::LoadTileMap() {
	OPENFILENAME ofn;
	wchar_t szFileName[MAX_PATH] = L"";

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = _hwnd;
	ofn.lpstrFilter = L"Ÿ�ϸ� ���� (*.tilemap)\0*.tilemap\0��� ���� (*.*)\0*.*\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_FILEMUSTEXIST;
	ofn.lpstrDefExt = L"tilemap";

	if (GetOpenFileName(&ofn)) {
		// ���� �̸��� ���õǾ����� �ε�
		std::wstring wFileName = szFileName;
		std::wstring fileName(wFileName.begin(), wFileName.end());

		std::wifstream file(fileName);
		if (file.is_open()) {

			wchar_t comma;
			// �׸��� ũ�� �ε�
			int width, height, tileMapW, tileMapH;
			file >> width >> comma >> height >> comma >> tileMapW >> comma >> tileMapH;

			// ���� �׸��� ũ��� �ٸ��� ���
			if (width != GridWidth || height != GridHeight) {
				MessageBox(_hwnd,
					L"�ε�� Ÿ�ϸ��� ũ�Ⱑ ���� �׸���� �ٸ��ϴ�. �Ϻ� Ÿ���� �߸� �� �ֽ��ϴ�.",
					L"���", MB_OK | MB_ICONWARNING);
			}

			// ���̾� ������ �ʱ�ȭ
			for (int i = 0; i < _layerCount; i++) {
				_layer[i].mainGrid.assign(GridWidth * GridHeight, -1);
			}

			// ���̾ ������ �ε�
			std::wstring line;
			while (std::getline(file, line)) {
				std::wistringstream iss(line);

				// ���̾� ��ȣ Ȯ��
				int layerIndex, tileCount;
				if (iss >> layerIndex >> comma >> tileCount)
				{
					if (layerIndex < 0 || layerIndex >= _layerCount) {
						MessageBox(_hwnd, L"�߸��� ���̾� �����Ͱ� �߰ߵǾ����ϴ�.", L"����", MB_OK | MB_ICONERROR);
						break;
					}

					// Ÿ�� ������ �б�
					for(int index = 0; index < tileCount; ++index)
					{
						std::getline(file, line);

						if (line.empty() == false)
						{
							std::wistringstream tileStream(line);
							int x, y, tileIndex;
							wchar_t comma;

							if (tileStream >> x >> comma >> y >> comma >> tileIndex) {
								if (x >= 0 && x < GridWidth && y >= 0 && y < GridHeight) {
									_layer[layerIndex].mainGrid[y * GridWidth + x] = tileIndex;
								}
							}
						}
					}
				}
			}

			file.close();
			MessageBox(_hwnd, L"Ÿ�ϸ��� �ε�Ǿ����ϴ�.", L"�ε� �Ϸ�", MB_OK | MB_ICONINFORMATION);
		}
		else {
			MessageBox(_hwnd, L"������ �ε��� �� �����ϴ�.", L"����", MB_OK | MB_ICONERROR);
		}
	}
}

bool TileMapMain::IsValidTile(int tileIndex)
{
	if (_selectedLayer < 0 || _selectedLayer >= _layerCount)
		return false;

	// ������ Ÿ���� �׸��忡 �׸���
	int tileX = tileIndex % TileMapWidth;
	int tileY = tileIndex / TileMapWidth;

	for (const auto& tile : _validLayerTiles[_selectedLayer])
	{
		if (tileX >= tile.minX && tileX <= tile.maxX && tileY >= tile.minY && tileY <= tile.maxY)
			return true;
	}

	return false;
}

int TileMapMain::TileLayer::GetValidCount()
{
	int count = 0;
	for (auto iter : mainGrid)
	{
		if (iter >= 0) count++;
	}
	return count;
}
