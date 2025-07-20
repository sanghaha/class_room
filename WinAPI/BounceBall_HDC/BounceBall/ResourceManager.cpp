#include "pch.h"
#include "ResourceManager.h"
#include "DXBitmap.h"
#include "Texture.h"	
#include "Sprite.h"
#include "AnimSprite.h"
#include "Game.h"
#include "Sound.h"

void ResourceManager::Init(HWND hwnd, fs::path directory)
{
	_hwnd = hwnd;
	_resourcePath = directory;


	{
		createSpriteNameInfo("Ball", 1, 3, L"Ball");
		createSpriteNameInfo("Block", 3, 17, L"Block");
		createSpriteNameInfo("Star", 1, 3, L"Star");
		_spriteNames.emplace("EatStarEffect", SpriteInfo{0, 0, L"EatStarEffect", true});
		_spriteNames.emplace("DeadBall", SpriteInfo{ 0, 0, L"DeadBall", true });
	}

	// font
	{
		loadFont();
	}

	// brush
	{
		createBrushes();
	}
}

void ResourceManager::Update(float deltaTime)
{

}

void ResourceManager::Destroy()
{
	for (auto& [key, res] : _bitmap)
	{
		delete res;
	}
	_bitmap.clear();
}

DXBitmap* ResourceManager::LoadDXBitmap(wstring key, wstring path, int32 countX, int32 countY, int32 transparent)
{
	if (_bitmap.find(key) != _bitmap.end())
	{
		// 이미 존재하는 키라면 리턴
		return _bitmap[key];
	}

	fs::path fullPath = _resourcePath / "Image/" / path;

	DXBitmap* bitmap = new DXBitmap();
	bitmap->Load(fullPath.c_str(), countX, countY, transparent);
	_bitmap[key] = bitmap;
	return bitmap;
}

DXBitmap* ResourceManager::GetDXBitmap(wstring key)
{
	if (_bitmap.find(key) != _bitmap.end())
	{
		// 이미 존재하는 키라면 리턴
		return _bitmap[key];
	}

	return nullptr;
}

const SpriteInfo* ResourceManager::GetSpriteInfo(string key)
{
	if (_spriteNames.find(key) != _spriteNames.end())
	{
		// 이미 존재하는 키라면 리턴
		return &_spriteNames[key];
	}
	return nullptr;
}

Sound* ResourceManager::LoadSound(const wstring& key, const wstring& path)
{
	if (_sounds.find(key) != _sounds.end())
		return _sounds[key];

	fs::path fullPath = _resourcePath / path;

	Sound* sound = new Sound();
	sound->LoadWave(fullPath);
	_sounds[key] = sound;

	return sound;
}

//void ResourceManager::PlayWAVSound(string fileName)
//{
//	fs::path fullPath = _resourcePath / L"Sound/" / fileName;
//	PlaySound(fullPath.c_str(), NULL, SND_FILENAME | SND_ASYNC);
//}

bool ResourceManager::loadFont()
{
	return false;
}

bool ResourceManager::createBrushes()
{
	return false;
}

void ResourceManager::createSpriteNameInfo(string spriteName, int32 xCount, int32 totalCount, wstring bitmapKey)
{
	for (int32 i = 0; i < totalCount; ++i)
	{
		int32 x = i % xCount;
		int32 y = i / xCount;
		_spriteNames.emplace(std::format("{0}_{1}", spriteName, i), SpriteInfo{ x, y, bitmapKey, true });
	}
}


