#include "pch.h"
#include "BaseResource.h"
#include "ResourceManager.h"
#include "DXBitmap.h"

BaseResource::BaseResource(string key, int32 width, int32 height) : _bitmapKey(key)
{
	SetBitmapKey(key, width, height);
}

BaseResource::~BaseResource()
{
}

void BaseResource::SetBitmapKey(string key, int32 width, int32 height)
{
	_bitmap = ResourceManager::GetInstance()->GetDXBitmap(key);

	_size.Width = width != 0 ? width : _bitmap->GetFrameSize().Width;
	_size.Height = height != 0 ? height : _bitmap->GetFrameSize().Height;
}
