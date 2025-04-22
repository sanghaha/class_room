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

	// ���� �Լ�
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
		std::vector<int> mainGrid; // ���� â �׸��� ������
		int GetValidCount();
	};
	TileLayer _layer[_layerCount]; // ���̾� �迭 ����

	vector<TileInfo> _validLayerTiles[_layerCount];	// �ش� ���̾�� ���Ǵ� Ÿ�� ����

	bool _isDragging = false;      // �巡�� ���� �ʱ�ȭ
	bool _isDrawCurrLayer = false;
};

