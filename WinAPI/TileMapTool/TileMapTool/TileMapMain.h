#pragma once
#include <windows.h>

class TileMapMain
{
public:
	void Init(HWND hwnd, std::wstring path);
	void Update();
	void Render();

private:
	void DrawMainGrid(HDC hdc);
	void DrawTileOnGrid(HDC hdc, int layer, int gridX, int gridY);

	// 파일 함수
	void SaveTileMap();
	void LoadTileMap();

	bool IsValidTile(int tileIndex);

private:
	HWND	_hwnd;
	RECT	_rect;
	HDC		_hdc = {};
	HDC		_hdcBack = {};
	HBITMAP _bmpBack = {};

	HDC		_hdcBitmap = {};
	HBITMAP _bitmap = 0;
	int32 _transparent = 0;
	int32 _sizeX = 0;
	int32 _sizeY = 0;

	const static int _layerCount = 3;
	int _selectedLayer = 0;
	struct TileLayer
	{
		std::vector<int> mainGrid; // 메인 창 그리드 데이터
		int GetValidCount();
	};
	TileLayer _layer[_layerCount]; // 레이어 배열 정의

	vector<TileInfo> _validLayerTiles[_layerCount];	// 해당 레이어에서 허용되는 타일 정보

	bool _isDragging = false;      // 드래그 상태 초기화
	bool _isDrawCurrLayer = false;
};

