#include "pch.h"
#include "ResourceBase.h"
#include "ResourceManager.h"

ResourceBase::ResourceBase(wstring key, int32 width, int32 height)
{
	_bitmapInfo = ResourceManager::GetInstance()->GetHBitmap(key);

	_size.w = width != 0 ? width : _bitmapInfo->size.w;
	_size.h = height != 0 ? height : _bitmapInfo->size.h;
}

ResourceBase::~ResourceBase()
{
}
