#pragma once

class DXBitmap;

class BaseResource
{
public:
	BaseResource(string key, int32 width, int32 height);
	virtual ~BaseResource();

	virtual void Update(float deltaTime) {}
	virtual void Render(ID2D1RenderTarget* renderTarget, Vector pos) {}

	Size GetSize() { return _size; }
	void SetBitmapKey(string key, int32 width = 0, int32 height = 0);
	DXBitmap* GetBitmap() { return _bitmap; }

protected:
	DXBitmap* _bitmap = nullptr;
	Size		_size = {};
	string		_bitmapKey;
};

