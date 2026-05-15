#include "pch.h"
#include "ResourceManager.h"
#include "Texture.h"
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
		_spriteNames.emplace("EatStarEffect", SpriteInfo{0, 0, 2.0f, false, L"EatStarEffect"});
		_spriteNames.emplace("DeadBall", SpriteInfo{ 0, 0, 2.0f, false, L"DeadBall"});
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

	for (auto& [key, sound] : _sounds)
	{
		delete sound;
	}
	_sounds.clear();

	if (redPen)
	{
		::DeleteObject(redPen);
		redPen = nullptr;
	}
}

Texture* ResourceManager::LoadTexture(wstring key, wstring path, int32 countX, int32 countY, int32 transparent, bool enableFlip)
{
	if (_bitmap.find(key) != _bitmap.end())
	{
		// 이미 존재하는 키라면 리턴
		return _bitmap[key];
	}

	fs::path fullPath = _resourcePath / "Image/" / path;

	Texture* bitmap = new Texture();
	bitmap->Load(fullPath.c_str(), countX, countY, transparent, enableFlip);
	_bitmap[key] = bitmap;
	return bitmap;
}

Texture* ResourceManager::GetTexture(wstring key)
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
	//FR_PRIVATE: 시스템 전체가 아닌 현재 프로세스에서만 사용
	AddFontResourceEx((_resourcePath / L"Font/MaplestoryBold.ttf").c_str(), FR_PRIVATE, 0);

	auto createFont = [this](FontSize size) {

		HFONT hFont = CreateFont(
			(int32)size, 0, 0, 0, FW_NORMAL,
			FALSE, FALSE, FALSE,
			DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			ANTIALIASED_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE,
			L"메이플스토리"  // Family 이름!
		);
		_fonts[size] = hFont;
		};

	createFont(FontSize::FONT_12);
	createFont(FontSize::FONT_20);
	createFont(FontSize::FONT_30);

	return true;
}

bool ResourceManager::createBrushes()
{
	redPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	return false;
}

void ResourceManager::createSpriteNameInfo(string spriteName, int32 xCount, int32 totalCount, wstring bitmapKey)
{
	for (int32 i = 0; i < totalCount; ++i)
	{
		int32 x = i % xCount;
		int32 y = i / xCount;
		_spriteNames.emplace(std::format("{0}_{1}", spriteName, i), SpriteInfo{ x, y, 0, false, bitmapKey});
	}
}


