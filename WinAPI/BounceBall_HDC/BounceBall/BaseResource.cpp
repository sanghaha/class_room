#include "pch.h"
#include "BaseResource.h"
#include "ResourceManager.h"
#include "DXBitmap.h"

BaseResource::BaseResource(wstring key, int32 width, int32 height) : _bitmapKey(key)
{
	_bitmap = ResourceManager::GetInstance()->GetDXBitmap(key);

	_size.Width = width != 0 ? width : _bitmap->GetFrameSize().Width;
	_size.Height = height != 0 ? height : _bitmap->GetFrameSize().Height;
}

BaseResource::~BaseResource()
{
}

void BaseResource::Render(HDC renderTarget, Vector pos)
{
}
