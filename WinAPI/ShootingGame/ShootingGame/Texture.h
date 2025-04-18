#pragma once
#include "ResourceBase.h"

class Texture
{
public:
	Texture();
	virtual ~Texture();
	void Render(HDC hdc, Pos pos);

	void Load(HWND hwnd, wstring path, int32 transparent);
	Size GetSize() { return Size{ _sizeX, _sizeY }; }
	
private:
	int32 _sizeX = 0;
	int32 _sizeY = 0;
	HDC _hdc = 0;
	HBITMAP _bitmap = 0;
	int32 _transparent = 0;
};

