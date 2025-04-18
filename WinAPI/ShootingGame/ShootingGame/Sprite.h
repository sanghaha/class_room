#pragma once

class Sprite
{
public:
	Sprite();
	virtual ~Sprite();

	void Render(HDC hdc, Pos pos, int32 indexX, int32 indexY);
	void Load(HWND hwnd, wstring path, int32 transparent, int32 countX, int32 countY, bool loop);
	
	int32 GetCountX() const { return _countX; }
	int32 GetCountY() const { return _countY; }
	Size GetSize() { return _renderSize; }
	bool IsLoop() const { return _loop; }

private:
	int32 _sizeX = 0;
	int32 _sizeY = 0;
	HDC _hdc = 0;
	HBITMAP _bitmap = 0;
	int32 _transparent = 0;
	int32 _countX = 0;
	int32 _countY = 0;
	Size _renderSize = {};
	bool _loop = false;
};



