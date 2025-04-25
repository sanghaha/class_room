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


	// 그리드 초기화
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
		// 클릭한 그리드 위치 계산
		int x = mousePos.x / TileSize;
		int y = mousePos.y / TileSize;

		// 그리드 범위 확인
		if (x >= 0 && x < GridWidth && y >= 0 && y < GridHeight && ToolManager::GetInstance()->GetSelectedTileIndex() >= 0)
		{
			// 선택된 타일을 그리드에 배치
			if (IsValidTile(ToolManager::GetInstance()->GetSelectedTileIndex()))
			{
				_layer[_selectedLayer].mainGrid[y * GridWidth + x] = ToolManager::GetInstance()->GetSelectedTileIndex();
			}
		}
	}

	if (InputManager::GetInstance()->GetButtonDown(KeyType::RightMouse))
	{
		POINT mousePos = InputManager::GetInstance()->GetMainMousePos();
		// 클릭한 그리드 위치 계산
		int x = mousePos.x / TileSize;
		int y = mousePos.y / TileSize;

		// 그리드 범위 확인
		if (x >= 0 && x < GridWidth && y >= 0 && y < GridHeight && ToolManager::GetInstance()->GetSelectedTileIndex() >= 0)
		{
			// 선택된 타일을 그리드에 배치
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
	DrawMainGrid(_hdcBack); // 백 버퍼에 그리기

	{
		std::wstring str = std::format(L"Layer:{0}, State:{1}", _selectedLayer, _isDrawCurrLayer);
		//::TextOut(_hdcBack, 5, 10, str.c_str(), static_cast<int>(str.size()));
	}


	// Double Buffering
	::BitBlt(_hdc, 0, 0, _rect.right, _rect.bottom, _hdcBack, 0, 0, SRCCOPY); // 비트 블릿 : 고속 복사
	::PatBlt(_hdcBack, 0, 0, _rect.right, _rect.bottom, WHITENESS);
}

void TileMapMain::DrawMainGrid(HDC hdc)
{
	HPEN hPen = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
	HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

	// 격자 그리기
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

	// 타일 그리기
	for (int i = 0; i < _layerCount; i++) 
	{
		if (_isDrawCurrLayer && i != _selectedLayer)
			continue;

		// 레이어별 타일 그리기
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

	// 선택한 타일을 그리드에 그리기
	int tileX = tileIndex % TileMapWidth;
	int tileY = tileIndex / TileMapWidth;

	// 흰색(RGB(255, 255, 255))을 투명색으로 처리하여 비트맵 그리기
	TransparentBlt(
		hdc,
		gridX * TileSize, gridY * TileSize, TileSize, TileSize, // 대상 위치 및 크기
		_hdcBitmap,
		tileX * TileSize, tileY * TileSize, TileSize, TileSize, // 원본 위치 및 크기
		_transparent
	);
}


void TileMapMain::SaveTileMap() {
	OPENFILENAME ofn;
	wchar_t szFileName[MAX_PATH] = L"";

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = _hwnd;
	ofn.lpstrFilter = L"타일맵 파일 (*.tilemap)\0*.tilemap\0모든 파일 (*.*)\0*.*\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	ofn.lpstrDefExt = L"tilemap";

	if (GetSaveFileName(&ofn)) {
		// 파일 이름이 선택되었으면 저장
		std::wstring wFileName = szFileName;
		std::wstring fileName(wFileName.begin(), wFileName.end());

		std::wofstream file(fileName);
		if (file.is_open()) {
			// 그리드 크기 저장
			file << GridWidth << "," << GridHeight << "," << TileMapWidth << "," << TileMapHeight << std::endl;

			// 타일 데이터 저장
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
			MessageBox(_hwnd, L"타일맵이 저장되었습니다.", L"저장 완료", MB_OK | MB_ICONINFORMATION);
		}
		else {
			MessageBox(_hwnd, L"파일을 저장할 수 없습니다.", L"오류", MB_OK | MB_ICONERROR);
		}
	}
}

void TileMapMain::LoadTileMap() {
	OPENFILENAME ofn;
	wchar_t szFileName[MAX_PATH] = L"";

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = _hwnd;
	ofn.lpstrFilter = L"타일맵 파일 (*.tilemap)\0*.tilemap\0모든 파일 (*.*)\0*.*\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_FILEMUSTEXIST;
	ofn.lpstrDefExt = L"tilemap";

	if (GetOpenFileName(&ofn)) {
		// 파일 이름이 선택되었으면 로드
		std::wstring wFileName = szFileName;
		std::wstring fileName(wFileName.begin(), wFileName.end());

		std::wifstream file(fileName);
		if (file.is_open()) {

			wchar_t comma;
			// 그리드 크기 로드
			int width, height, tileMapW, tileMapH;
			file >> width >> comma >> height >> comma >> tileMapW >> comma >> tileMapH;

			// 기존 그리드 크기와 다르면 경고
			if (width != GridWidth || height != GridHeight) {
				MessageBox(_hwnd,
					L"로드된 타일맵의 크기가 현재 그리드와 다릅니다. 일부 타일이 잘릴 수 있습니다.",
					L"경고", MB_OK | MB_ICONWARNING);
			}

			// 레이어 데이터 초기화
			for (int i = 0; i < _layerCount; i++) {
				_layer[i].mainGrid.assign(GridWidth * GridHeight, -1);
			}

			// 레이어별 데이터 로드
			std::wstring line;
			while (std::getline(file, line)) {
				std::wistringstream iss(line);

				// 레이어 번호 확인
				int layerIndex, tileCount;
				if (iss >> layerIndex >> comma >> tileCount)
				{
					if (layerIndex < 0 || layerIndex >= _layerCount) {
						MessageBox(_hwnd, L"잘못된 레이어 데이터가 발견되었습니다.", L"오류", MB_OK | MB_ICONERROR);
						break;
					}

					// 타일 데이터 읽기
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
			MessageBox(_hwnd, L"타일맵이 로드되었습니다.", L"로드 완료", MB_OK | MB_ICONINFORMATION);
		}
		else {
			MessageBox(_hwnd, L"파일을 로드할 수 없습니다.", L"오류", MB_OK | MB_ICONERROR);
		}
	}
}

bool TileMapMain::IsValidTile(int tileIndex)
{
	if (_selectedLayer < 0 || _selectedLayer >= _layerCount)
		return false;

	// 선택한 타일을 그리드에 그리기
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
