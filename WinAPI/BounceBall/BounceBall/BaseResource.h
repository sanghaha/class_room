#pragma once

class DXBitmap;

class BaseResource
{
public:
	BaseResource(wstring key, int32 width, int32 height);
	virtual ~BaseResource();

	virtual void Update(float deltaTime) {}
	virtual void Render(ID2D1RenderTarget* renderTarget, Vector pos) abstract;
	Size GetSize() { return _size; }

protected:
	DXBitmap*	_bitmap = nullptr;
	Size		_size = {};
	wstring		_bitmapKey;
};

