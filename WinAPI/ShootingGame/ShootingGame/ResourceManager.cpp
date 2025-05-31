#include "pch.h"
#include "ResourceManager.h"
#include "Texture.h"	
#include "Sprite.h"

void ResourceManager::Init(HWND hwnd, fs::path directory)
{
	_hwnd = hwnd;
	_resourcePath = directory;
}

void ResourceManager::Update(float deltaTime)
{

}

void ResourceManager::Destroy()
{
	for (auto& [key, bitmap] : _bitmaps)
	{
		delete bitmap;
	}
	_bitmaps.clear();
}

const HBitmapInfo* ResourceManager::LoadHBitmap(wstring key, wstring path, int32 transparent, int32 countX, int32 countY, bool loop)
{
	if (_bitmaps.find(key) != _bitmaps.end())
	{
		// 이미 존재하는 키라면 리턴
		return _bitmaps[key];
	}

	fs::path fullPath = _resourcePath / path;

	HBitmapInfo* info = new HBitmapInfo();

	HDC hdc = ::GetDC(_hwnd);

	info->hdc = ::CreateCompatibleDC(hdc);
	info->bitmap = (HBITMAP)::LoadImageW(
		nullptr,
		fullPath.c_str(),
		IMAGE_BITMAP,
		0,
		0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION
	);

	info->transparent = transparent;
	info->countX = countX;
	info->countY = countY;
	info->loop = loop;

	if (info->bitmap == 0)
	{
		::MessageBox(_hwnd, path.c_str(), L"Invalid Texture Load", MB_OK);
		return nullptr;
	}

	HBITMAP prev = (HBITMAP)::SelectObject(info->hdc, info->bitmap);
	::DeleteObject(prev);

	BITMAP bit = {};
	::GetObject(info->bitmap, sizeof(BITMAP), &bit);

	info->size.w = bit.bmWidth;
	info->size.h = bit.bmHeight;
	
	_bitmaps[key] = info;
	return info;
}

const HBitmapInfo* ResourceManager::GetHBitmap(wstring key)
{
	if (_bitmaps.find(key) != _bitmaps.end())
	{
		// 이미 존재하는 키라면 리턴
		return _bitmaps[key];
	}

	return nullptr;
}