#pragma once

#include <string>

// Direct 2D
#include <d2d1.h>
#include <dwrite_1.h>
#include <dwrite_3.h>
#include <wincodec.h>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "windowscodecs.lib")

// C++ 20
#include <format>
#include <filesystem>
namespace fs = std::filesystem;

class Game
{
public:
	void Init(HWND hwnd);
	void Update();
	void Render();

	void LoadTexture(std::wstring path, int maxCountX, int maxCountY);

	ID2D1Factory* _dxFactory = nullptr;
	ID2D1HwndRenderTarget* _dxRenderTarget = nullptr;
	IWICImagingFactory* _wicFactory = nullptr;

	ID2D1Bitmap* _bitmap = nullptr;
	UINT	_bitmapSizeX = 0;
	UINT	_bitmapSizeY = 0;
	int	_maxCountX = 0;
	int	_maxCountY = 0;
	int	_frameSizeX = 0;
	int	_frameSizeY = 0;

	int _indexX = 0;
	int _indexY = 0;
};

